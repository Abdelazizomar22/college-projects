from __future__ import annotations

import argparse
from typing import List, Tuple, Union

import cv2
import numpy as np

from tracker import CentroidTracker


def parse_source(s: str) -> Union[int, str]:
    # allow webcam index like "0"
    if s.isdigit():
        return int(s)
    return s


def detect_objects(frame: np.ndarray, bg: cv2.BackgroundSubtractor, min_area: int) -> List[Tuple[int, int, int, int]]:
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.GaussianBlur(gray, (5, 5), 0)

    fg = bg.apply(gray)
    _, fg = cv2.threshold(fg, 200, 255, cv2.THRESH_BINARY)

    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    fg = cv2.morphologyEx(fg, cv2.MORPH_OPEN, kernel, iterations=1)
    fg = cv2.morphologyEx(fg, cv2.MORPH_CLOSE, kernel, iterations=2)

    contours, _ = cv2.findContours(fg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    boxes: List[Tuple[int, int, int, int]] = []
    for c in contours:
        area = cv2.contourArea(c)
        if area < min_area:
            continue
        x, y, w, h = cv2.boundingRect(c)
        boxes.append((x, y, w, h))
    return boxes


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", default="0", help="Webcam index (0) or path to video file")
    ap.add_argument("--min-area", type=int, default=900, help="Minimum contour area")
    ap.add_argument("--max-distance", type=int, default=60, help="Tracker association distance")
    args = ap.parse_args()

    source = parse_source(args.source)
    cap = cv2.VideoCapture(source)
    if not cap.isOpened():
        raise SystemExit("Failed to open video source")

    bg = cv2.createBackgroundSubtractorMOG2(history=300, varThreshold=32, detectShadows=False)
    tracker = CentroidTracker(max_distance=args.max_distance)

    while True:
        ok, frame = cap.read()
        if not ok:
            break

        boxes = detect_objects(frame, bg, args.min_area)
        tracks = tracker.update(boxes)

        # draw
        for t in tracks.values():
            x, y, w, h = t.bbox
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(
                frame,
                f"ID {t.track_id}",
                (x, max(0, y - 8)),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 255, 0),
                2,
                cv2.LINE_AA,
            )

        cv2.imshow("Object Detection + Tracking", frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

