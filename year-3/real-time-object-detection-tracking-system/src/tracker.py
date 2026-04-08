from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Tuple


@dataclass
class Track:
    track_id: int
    centroid: Tuple[int, int]
    bbox: Tuple[int, int, int, int]  # x, y, w, h
    missed: int = 0


class CentroidTracker:
    def __init__(self, max_distance: int = 60, max_missed: int = 15):
        self.max_distance = max_distance
        self.max_missed = max_missed
        self._next_id = 1
        self.tracks: Dict[int, Track] = {}

    def update(self, detections: List[Tuple[int, int, int, int]]) -> Dict[int, Track]:
        det_centroids = [((x + x + w) // 2, (y + y + h) // 2) for (x, y, w, h) in detections]

        if not self.tracks:
            for det, c in zip(detections, det_centroids):
                tid = self._next_id
                self._next_id += 1
                self.tracks[tid] = Track(track_id=tid, centroid=c, bbox=det)
            return self.tracks

        unmatched_tracks = set(self.tracks.keys())
        unmatched_dets = set(range(len(detections)))

        # Greedy matching by distance.
        pairs = []
        for tid, t in self.tracks.items():
            for di, c in enumerate(det_centroids):
                dx = t.centroid[0] - c[0]
                dy = t.centroid[1] - c[1]
                dist2 = dx * dx + dy * dy
                pairs.append((dist2, tid, di))
        pairs.sort(key=lambda x: x[0])

        for dist2, tid, di in pairs:
            if tid not in unmatched_tracks or di not in unmatched_dets:
                continue
            if dist2 > self.max_distance * self.max_distance:
                continue
            unmatched_tracks.remove(tid)
            unmatched_dets.remove(di)
            self.tracks[tid].centroid = det_centroids[di]
            self.tracks[tid].bbox = detections[di]
            self.tracks[tid].missed = 0

        # Increase missed count for unmatched tracks.
        to_delete = []
        for tid in unmatched_tracks:
            self.tracks[tid].missed += 1
            if self.tracks[tid].missed > self.max_missed:
                to_delete.append(tid)
        for tid in to_delete:
            del self.tracks[tid]

        # Create new tracks for unmatched detections.
        for di in unmatched_dets:
            tid = self._next_id
            self._next_id += 1
            self.tracks[tid] = Track(track_id=tid, centroid=det_centroids[di], bbox=detections[di])

        return self.tracks

