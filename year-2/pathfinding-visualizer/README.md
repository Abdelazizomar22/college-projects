# Pathfinding Visualizer (C++ / SFML)

Interactive grid-based visualizer demonstrating shortest-path algorithms:
- BFS
- Dijkstra
- A*

Allows placing walls and observing algorithm exploration and final shortest path.

## Context
- Academic year: 2nd year
- Course: Data Structures and Algorithms
- Developed during: April 2023

## Controls
- Left click: toggle wall
- Shift + Left click: set start
- Ctrl + Left click: set end
- B: run BFS
- D: run Dijkstra
- A: run A*
- R: reset visited/path (keep walls)
- C: clear walls + reset
- Esc: quit

## Build notes
This project uses SFML (Graphics/Window/System). Install SFML and make sure your compiler can find headers and libraries.

If you use CMake:
- Configure with `-DSFML_DIR=...` if SFML isn't on a standard path.

