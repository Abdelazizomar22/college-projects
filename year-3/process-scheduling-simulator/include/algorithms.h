#pragma once

#include "models.h"

typedef struct {
  Slice* slices;
  int slice_count;
  Metrics* metrics;
  int metrics_count;
  int total_time;
  int busy_time;
} ScheduleResult;

void free_result(ScheduleResult* r);

ScheduleResult run_fcfs(const Process* p, int n);
ScheduleResult run_sjf_np(const Process* p, int n);
ScheduleResult run_priority_np(const Process* p, int n);
ScheduleResult run_round_robin(const Process* p, int n, int quantum);

