#include "report.h"

#include <stdio.h>

static void print_gantt(const ScheduleResult* r) {
  printf("Gantt:\\n");
  printf("  |");
  for (int i = 0; i < r->slice_count; i++) {
    printf(" P%d %d-%d |", r->slices[i].pid, r->slices[i].start, r->slices[i].end);
  }
  printf("\\n");
}

static void print_metrics(const ScheduleResult* r) {
  double avg_wait = 0.0;
  double avg_turn = 0.0;
  for (int i = 0; i < r->metrics_count; i++) {
    avg_wait += r->metrics[i].waiting;
    avg_turn += r->metrics[i].turnaround;
  }
  if (r->metrics_count > 0) {
    avg_wait /= r->metrics_count;
    avg_turn /= r->metrics_count;
  }

  printf("Per-process metrics:\\n");
  printf("  PID  Arrival  Burst  Completion  Waiting  Turnaround\\n");
  for (int i = 0; i < r->metrics_count; i++) {
    const Metrics* m = &r->metrics[i];
    printf("  %-4d %-8d %-6d %-11d %-7d %-10d\\n", m->pid, m->arrival, m->burst, m->completion, m->waiting,
           m->turnaround);
  }

  double util = (r->total_time <= 0) ? 0.0 : (100.0 * (double)r->busy_time / (double)r->total_time);
  printf("Summary:\\n");
  printf("  Avg waiting time: %.2f\\n", avg_wait);
  printf("  Avg turnaround:   %.2f\\n", avg_turn);
  printf("  CPU utilization:  %.2f%%\\n", util);
}

void print_result(const char* title, const ScheduleResult* r) {
  printf("\\n=== %s ===\\n", title);
  if (!r || r->slice_count == 0) {
    printf("(no schedule)\\n");
    return;
  }
  print_gantt(r);
  print_metrics(r);
}

