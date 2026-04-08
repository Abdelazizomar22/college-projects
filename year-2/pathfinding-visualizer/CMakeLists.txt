cmake_minimum_required(VERSION 3.16)
project(pathfinding_visualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pathfinding_visualizer
  src/main.cpp
  src/Grid.cpp
  src/Algorithms.cpp
  src/MinHeap.cpp
  src/Queue.cpp
  src/Stack.cpp
)

target_include_directories(pathfinding_visualizer PRIVATE include)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(pathfinding_visualizer PRIVATE sfml-graphics sfml-window sfml-system)

