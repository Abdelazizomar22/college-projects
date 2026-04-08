# Approach

## Detection
1) Convert frame to grayscale and apply Gaussian blur
2) Background subtraction (MOG2) to get a foreground mask
3) Threshold + morphological operations to reduce noise
4) Find contours and filter by area

## Tracking
A simple centroid tracker assigns an ID to each detection by matching the closest centroids between frames with a distance threshold.

