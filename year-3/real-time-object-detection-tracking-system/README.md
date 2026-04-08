# Real-Time Object Detection and Tracking (Python + OpenCV)

Real-time object detection using classical image processing (background subtraction + morphology + contour filtering), plus lightweight centroid tracking across frames.

## Context
- Academic year: 3rd year
- Course: Image Processing
- Developed during: June 2024

## Features
- Webcam or video file input
- Foreground segmentation (MOG2 background subtraction)
- Noise reduction (blur + morphology)
- Contour-based detection with area filtering
- Simple ID tracking via centroid association

## Setup
Create a virtual environment, then:

  pip install -r requirements.txt

## Run
Webcam:

  python src/main.py --source 0

Video file:

  python src/main.py --source path/to/video.mp4

Keys:
- q: quit
