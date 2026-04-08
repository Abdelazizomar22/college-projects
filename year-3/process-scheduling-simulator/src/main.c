#include "algorithms.h"
#include "parser.h"
#include "report.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(void) {
  printf("Usage:\\n");
  printf("  scheduler --algo <fcfs|sjf|priority|rr|all> --input <file> [--quantum N]\\n");
}

static const char* arg_value(int* i, int argc, char** argv) {
  if (*i + 1 >= argc) return NULL;
  (*i)++;
  return argv[*i];
}

int main(int argc, char** argv) {
  const char* algo = NULL;
  const char* input = NULL;
  int quantum = 2;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--algo") == 0) {
      algo = arg_value(&i, argc, argv);
    } else if (strcmp(argv[i], "--input") == 0) {
      input = arg_value(&i, argc, argv);
    } else if (strcmp(argv[i], "--quantum") == 0) {
      const char* v = arg_value(&i, argc, argv);
      if (v) quantum = atoi(v);
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      usage();
      return 0;
    } else {
      printf("Unknown arg: %s\\n", argv[i]);
      usage();
      return 1;
    }
  }

  if (!algo || !input) {
    usage();
    return 1;
  }
  if (quantum <= 0) quantum = 2;

  Process* p = NULL;
  int n = 0;
  if (load_processes(input, &p, &n) != 0) {
    printf("Failed to load input file.\\n");
    return 1;
  }

  if (strcmp(algo, "fcfs") == 0) {
    ScheduleResult r = run_fcfs(p, n);
    print_result("FCFS", &r);
    free_result(&r);
  } else if (strcmp(algo, "sjf") == 0) {
    ScheduleResult r = run_sjf_np(p, n);
    print_result("SJF (non-preemptive)", &r);
    free_result(&r);
  } else if (strcmp(algo, "priority") == 0) {
    ScheduleResult r = run_priority_np(p, n);
    print_result("Priority (non-preemptive)", &r);
    free_result(&r);
  } else if (strcmp(algo, "rr") == 0) {
    ScheduleResult r = run_round_robin(p, n, quantum);
    print_result("Round Robin", &r);
    free_result(&r);
  } else if (strcmp(algo, "all") == 0) {
    ScheduleResult r1 = run_fcfs(p, n);
    print_result("FCFS", &r1);
    free_result(&r1);

    ScheduleResult r2 = run_sjf_np(p, n);
    print_result("SJF (non-preemptive)", &r2);
    free_result(&r2);

    ScheduleResult r3 = run_priority_np(p, n);
    print_result("Priority (non-preemptive)", &r3);
    free_result(&r3);

    ScheduleResult r4 = run_round_robin(p, n, quantum);
    print_result("Round Robin", &r4);
    free_result(&r4);
  } else {
    printf("Unknown algo.\\n");
    usage();
    free_processes(p);
    return 1;
  }

  free_processes(p);
  return 0;
}

