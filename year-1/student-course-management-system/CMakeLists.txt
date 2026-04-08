cmake_minimum_required(VERSION 3.16)
project(student_course_management_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(scms
  src/main.cpp
  src/App.cpp
  src/Storage.cpp
  src/Utils.cpp
)

target_include_directories(scms PRIVATE include)

