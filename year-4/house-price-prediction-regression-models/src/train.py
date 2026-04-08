from __future__ import annotations

import argparse
from dataclasses import dataclass
from typing import Dict, Tuple

import joblib
import numpy as np
import pandas as pd
from sklearn.compose import ColumnTransformer
from sklearn.impute import SimpleImputer
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import KFold, cross_val_score
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import OneHotEncoder
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor


@dataclass
class ModelResult:
    name: str
    cv_rmse_mean: float
    cv_rmse_std: float


def build_preprocess(X: pd.DataFrame) -> ColumnTransformer:
    numeric_cols = X.select_dtypes(include=["number"]).columns.tolist()
    categorical_cols = [c for c in X.columns if c not in numeric_cols]

    numeric_pipe = Pipeline(
        steps=[
            ("imputer", SimpleImputer(strategy="median")),
        ]
    )
    categorical_pipe = Pipeline(
        steps=[
            ("imputer", SimpleImputer(strategy="most_frequent")),
            ("onehot", OneHotEncoder(handle_unknown="ignore")),
        ]
    )

    return ColumnTransformer(
        transformers=[
            ("num", numeric_pipe, numeric_cols),
            ("cat", categorical_pipe, categorical_cols),
        ]
    )


def cv_rmse(model: Pipeline, X: pd.DataFrame, y: pd.Series, seed: int = 42) -> Tuple[float, float]:
    cv = KFold(n_splits=5, shuffle=True, random_state=seed)
    scores = cross_val_score(model, X, y, scoring="neg_root_mean_squared_error", cv=cv)
    rmse = -scores
    return float(rmse.mean()), float(rmse.std())


def compare_models(X: pd.DataFrame, y: pd.Series) -> Dict[str, Pipeline]:
    pre = build_preprocess(X)
    models: Dict[str, Pipeline] = {}

    models["LinearRegression"] = Pipeline(
        steps=[
            ("pre", pre),
            ("model", LinearRegression()),
        ]
    )

    models["DecisionTree"] = Pipeline(
        steps=[
            ("pre", pre),
            ("model", DecisionTreeRegressor(random_state=42)),
        ]
    )

    models["RandomForest"] = Pipeline(
        steps=[
            ("pre", pre),
            ("model", RandomForestRegressor(n_estimators=300, random_state=42, n_jobs=-1)),
        ]
    )

    return models


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--train", required=True, help="Path to train.csv")
    ap.add_argument("--target", default="SalePrice", help="Target column name")
    ap.add_argument("--save", default="", help="Optional path to save best model (joblib)")
    args = ap.parse_args()

    df = pd.read_csv(args.train)
    if args.target not in df.columns:
        raise SystemExit(f"Target column not found: {args.target}")

    y = df[args.target]
    X = df.drop(columns=[args.target])

    models = compare_models(X, y)

    results = []
    best_name = None
    best_rmse = float("inf")

    for name, pipe in models.items():
        mean_rmse, std_rmse = cv_rmse(pipe, X, y)
        results.append(ModelResult(name=name, cv_rmse_mean=mean_rmse, cv_rmse_std=std_rmse))
        if mean_rmse < best_rmse:
            best_rmse = mean_rmse
            best_name = name

    results.sort(key=lambda r: r.cv_rmse_mean)
    print("Cross-validation RMSE (lower is better):")
    for r in results:
        print(f"- {r.name:15s}  RMSE={r.cv_rmse_mean:.4f}  (std={r.cv_rmse_std:.4f})")

    if not best_name:
        return 0

    best_model = models[best_name]
    print(f"\\nBest model: {best_name}")

    # Fit on full training data (for saving)
    best_model.fit(X, y)

    # Quick train RMSE for sanity (not a validation metric)
    preds = best_model.predict(X)
    train_rmse = mean_squared_error(y, preds, squared=False)
    print(f"Train RMSE (sanity): {train_rmse:.4f}")

    if args.save:
        joblib.dump(best_model, args.save)
        print(f"Saved model to: {args.save}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

