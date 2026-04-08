# Process Scheduling Simulator (C)

CPU scheduling simulator implementing and comparing:
- FCFS
- SJF (non-preemptive)
- Priority (non-preemptive, lower number = higher priority)
- Round Robin (time quantum configurable)

Outputs a text Gantt chart and key metrics (waiting time, turnaround time, CPU utilization).

## Context
- Academic year: 3rd year
- Course: Operating Systems
- Developed during: January 2024

## Build & Run
Requirements: a C compiler (e.g., MinGW gcc) available on your PATH.

From this project folder:

  gcc -std=c11 -O2 -I include src\*.c -o scheduler.exe
  .\scheduler.exe --algo all --input data\processes.txt --quantum 2

## Input format
One process per line:

  pid arrival burst priority

Example: 1 0 5 2
