#pragma once

typedef struct {
  int pid;
  int arrival;
  int burst;
  int priority;
} Process;

typedef struct {
  int pid;
  int start;
  int end;
} Slice;

typedef struct {
  int pid;
  int arrival;
  int burst;
  int completion;
  int waiting;
  int turnaround;
} Metrics;

