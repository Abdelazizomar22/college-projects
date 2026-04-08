cmake_minimum_required(VERSION 3.16)
project(process_scheduling_simulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(scheduler
  src/main.c
  src/parser.c
  src/algorithms.c
  src/report.c
)

target_include_directories(scheduler PRIVATE include)

if (UNIX)
  target_link_libraries(scheduler PRIVATE pthread)
  target_compile_definitions(scheduler PRIVATE SCHED_HAS_PTHREAD=1)
endif()

