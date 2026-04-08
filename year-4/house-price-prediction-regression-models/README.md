# House Price Prediction using Regression Models (Kaggle)

ML project to predict house prices using multiple regression models and a complete pipeline:
- EDA
- Feature engineering
- Missing values handling
- Outlier checks
- Cross-validation + hyperparameter tuning

## Context
- Academic year: 4th year
- Course: Machine Learning
- Developed during: November 2025

## Kaggle Notebook
- Kaggle link: https://www.kaggle.com/code/abdelazizomarrr/house-price-prediction
- Notebook in this repo: notebooks/house-price-prediction.ipynb

## What to upload to GitHub (recommended)
- Notebook(s) in notebooks/ (EDA + experiments)
- Reproducible training script in src/ (so it can run without Jupyter)
- requirements.txt
- Small artifacts (plots) in docs/ or assets/ (optional)

Avoid uploading:
- Kaggle credentials (kaggle.json)
- The full dataset CSVs (they're large; share download steps instead)

## Setup
Create a virtual environment, then:

  pip install -r requirements.txt

## Data
Download the Kaggle Housing dataset and place:
- data/train.csv
- data/test.csv

## Train / Compare models

  python src/train.py --train data/train.csv --target SalePrice

If you want to save the best model:

  python src/train.py --train data/train.csv --target SalePrice --save models/best_model.joblib
