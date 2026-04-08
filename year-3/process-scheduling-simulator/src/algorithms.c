#include "algorithms.h"

#include <stdlib.h>
#include <string.h>

static int cmp_arrival_then_pid(const void* a, const void* b) {
  const Process* pa = (const Process*)a;
  const Process* pb = (const Process*)b;
  if (pa->arrival != pb->arrival) return pa->arrival - pb->arrival;
  return pa->pid - pb->pid;
}

static Metrics* init_metrics(const Process* p, int n) {
  Metrics* m = (Metrics*)calloc((size_t)n, sizeof(Metrics));
  for (int i = 0; i < n; i++) {
    m[i].pid = p[i].pid;
    m[i].arrival = p[i].arrival;
    m[i].burst = p[i].burst;
  }
  return m;
}

static int metrics_index(const Metrics* m, int n, int pid) {
  for (int i = 0; i < n; i++) {
    if (m[i].pid == pid) return i;
  }
  return -1;
}

static void push_slice(ScheduleResult* r, int pid, int start, int end) {
  if (start == end) return;
  int new_count = r->slice_count + 1;
  Slice* ns = (Slice*)realloc(r->slices, sizeof(Slice) * (size_t)new_count);
  if (!ns) return;  // best-effort
  r->slices = ns;
  r->slices[r->slice_count].pid = pid;
  r->slices[r->slice_count].start = start;
  r->slices[r->slice_count].end = end;
  r->slice_count = new_count;
}

static void finalize_metrics(ScheduleResult* r) {
  for (int i = 0; i < r->metrics_count; i++) {
    r->metrics[i].turnaround = r->metrics[i].completion - r->metrics[i].arrival;
    r->metrics[i].waiting = r->metrics[i].turnaround - r->metrics[i].burst;
  }
}

void free_result(ScheduleResult* r) {
  if (!r) return;
  free(r->slices);
  free(r->metrics);
  r->slices = NULL;
  r->metrics = NULL;
  r->slice_count = 0;
  r->metrics_count = 0;
  r->total_time = 0;
  r->busy_time = 0;
}

ScheduleResult run_fcfs(const Process* p, int n) {
  ScheduleResult r;
  memset(&r, 0, sizeof(r));
  if (!p || n <= 0) return r;

  Process* procs = (Process*)malloc(sizeof(Process) * (size_t)n);
  memcpy(procs, p, sizeof(Process) * (size_t)n);
  qsort(procs, (size_t)n, sizeof(Process), cmp_arrival_then_pid);

  r.metrics = init_metrics(procs, n);
  r.metrics_count = n;

  int t = 0;
  for (int i = 0; i < n; i++) {
    if (t < procs[i].arrival) t = procs[i].arrival;
    int start = t;
    t += procs[i].burst;
    push_slice(&r, procs[i].pid, start, t);
    r.busy_time += procs[i].burst;
    int idx = metrics_index(r.metrics, n, procs[i].pid);
    if (idx >= 0) r.metrics[idx].completion = t;
  }
  r.total_time = t;
  finalize_metrics(&r);
  free(procs);
  return r;
}

ScheduleResult run_sjf_np(const Process* p, int n) {
  ScheduleResult r;
  memset(&r, 0, sizeof(r));
  if (!p || n <= 0) return r;

  Process* procs = (Process*)malloc(sizeof(Process) * (size_t)n);
  memcpy(procs, p, sizeof(Process) * (size_t)n);
  qsort(procs, (size_t)n, sizeof(Process), cmp_arrival_then_pid);

  r.metrics = init_metrics(procs, n);
  r.metrics_count = n;

  int* done = (int*)calloc((size_t)n, sizeof(int));
  int completed = 0;
  int t = 0;

  while (completed < n) {
    int best = -1;
    for (int i = 0; i < n; i++) {
      if (done[i]) continue;
      if (procs[i].arrival > t) continue;
      if (best < 0 || procs[i].burst < procs[best].burst ||
          (procs[i].burst == procs[best].burst && procs[i].pid < procs[best].pid)) {
        best = i;
      }
    }

    if (best < 0) {
      // idle until next arrival
      int next_arrival = -1;
      for (int i = 0; i < n; i++) {
        if (done[i]) continue;
        if (next_arrival < 0 || procs[i].arrival < next_arrival) next_arrival = procs[i].arrival;
      }
      if (next_arrival < 0) break;
      t = next_arrival;
      continue;
    }

    int start = t;
    t += procs[best].burst;
    push_slice(&r, procs[best].pid, start, t);
    r.busy_time += procs[best].burst;
    done[best] = 1;
    completed++;
    int idx = metrics_index(r.metrics, n, procs[best].pid);
    if (idx >= 0) r.metrics[idx].completion = t;
  }

  r.total_time = t;
  finalize_metrics(&r);
  free(done);
  free(procs);
  return r;
}

ScheduleResult run_priority_np(const Process* p, int n) {
  ScheduleResult r;
  memset(&r, 0, sizeof(r));
  if (!p || n <= 0) return r;

  Process* procs = (Process*)malloc(sizeof(Process) * (size_t)n);
  memcpy(procs, p, sizeof(Process) * (size_t)n);
  qsort(procs, (size_t)n, sizeof(Process), cmp_arrival_then_pid);

  r.metrics = init_metrics(procs, n);
  r.metrics_count = n;

  int* done = (int*)calloc((size_t)n, sizeof(int));
  int completed = 0;
  int t = 0;

  while (completed < n) {
    int best = -1;
    for (int i = 0; i < n; i++) {
      if (done[i]) continue;
      if (procs[i].arrival > t) continue;
      if (best < 0 || procs[i].priority < procs[best].priority ||
          (procs[i].priority == procs[best].priority && procs[i].pid < procs[best].pid)) {
        best = i;
      }
    }

    if (best < 0) {
      int next_arrival = -1;
      for (int i = 0; i < n; i++) {
        if (done[i]) continue;
        if (next_arrival < 0 || procs[i].arrival < next_arrival) next_arrival = procs[i].arrival;
      }
      if (next_arrival < 0) break;
      t = next_arrival;
      continue;
    }

    int start = t;
    t += procs[best].burst;
    push_slice(&r, procs[best].pid, start, t);
    r.busy_time += procs[best].burst;
    done[best] = 1;
    completed++;
    int idx = metrics_index(r.metrics, n, procs[best].pid);
    if (idx >= 0) r.metrics[idx].completion = t;
  }

  r.total_time = t;
  finalize_metrics(&r);
  free(done);
  free(procs);
  return r;
}

typedef struct {
  int pid;
  int remaining;
  int arrival;
} RrState;

typedef struct {
  int* items;
  int head;
  int tail;
  int cap;
} IntQueue;

static void q_init(IntQueue* q, int cap) {
  q->items = (int*)malloc(sizeof(int) * (size_t)cap);
  q->head = 0;
  q->tail = 0;
  q->cap = cap;
}

static void q_free(IntQueue* q) {
  free(q->items);
  q->items = NULL;
  q->head = q->tail = q->cap = 0;
}

static int q_empty(const IntQueue* q) { return q->head == q->tail; }

static void q_push(IntQueue* q, int v) {
  q->items[q->tail] = v;
  q->tail = (q->tail + 1) % q->cap;
}

static int q_pop(IntQueue* q) {
  int v = q->items[q->head];
  q->head = (q->head + 1) % q->cap;
  return v;
}

ScheduleResult run_round_robin(const Process* p, int n, int quantum) {
  ScheduleResult r;
  memset(&r, 0, sizeof(r));
  if (!p || n <= 0) return r;
  if (quantum <= 0) quantum = 1;

  Process* procs = (Process*)malloc(sizeof(Process) * (size_t)n);
  memcpy(procs, p, sizeof(Process) * (size_t)n);
  qsort(procs, (size_t)n, sizeof(Process), cmp_arrival_then_pid);

  r.metrics = init_metrics(procs, n);
  r.metrics_count = n;

  RrState* st = (RrState*)calloc((size_t)n, sizeof(RrState));
  for (int i = 0; i < n; i++) {
    st[i].pid = procs[i].pid;
    st[i].remaining = procs[i].burst;
    st[i].arrival = procs[i].arrival;
  }

  IntQueue q;
  q_init(&q, n * 8 + 8);

  int t = 0;
  int next = 0;
  int finished = 0;

  while (finished < n) {
    while (next < n && procs[next].arrival <= t) {
      q_push(&q, next);
      next++;
    }

    if (q_empty(&q)) {
      if (next < n) {
        t = procs[next].arrival;
        continue;
      }
      break;
    }

    int idx = q_pop(&q);
    int run_for = st[idx].remaining < quantum ? st[idx].remaining : quantum;
    int start = t;
    t += run_for;
    st[idx].remaining -= run_for;
    push_slice(&r, st[idx].pid, start, t);
    r.busy_time += run_for;

    while (next < n && procs[next].arrival <= t) {
      q_push(&q, next);
      next++;
    }

    if (st[idx].remaining > 0) {
      q_push(&q, idx);
    } else {
      finished++;
      int midx = metrics_index(r.metrics, n, st[idx].pid);
      if (midx >= 0) r.metrics[midx].completion = t;
    }
  }

  r.total_time = t;
  finalize_metrics(&r);
  q_free(&q);
  free(st);
  free(procs);
  return r;
}

