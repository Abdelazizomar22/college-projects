#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int load_processes(const char* path, Process** out, int* out_count) {
  if (!path || !out || !out_count) return 1;
  *out = NULL;
  *out_count = 0;

  FILE* f = fopen(path, "r");
  if (!f) return 1;

  int capacity = 16;
  int count = 0;
  Process* p = (Process*)malloc(sizeof(Process) * capacity);
  if (!p) {
    fclose(f);
    return 1;
  }

  while (1) {
    Process tmp;
    int read = fscanf(f, "%d %d %d %d", &tmp.pid, &tmp.arrival, &tmp.burst, &tmp.priority);
    if (read == EOF) break;
    if (read != 4) {
      // Skip invalid line
      int c;
      do {
        c = fgetc(f);
      } while (c != '\n' && c != EOF);
      continue;
    }

    if (count >= capacity) {
      capacity *= 2;
      Process* np = (Process*)realloc(p, sizeof(Process) * capacity);
      if (!np) {
        free(p);
        fclose(f);
        return 1;
      }
      p = np;
    }
    p[count++] = tmp;
  }

  fclose(f);
  *out = p;
  *out_count = count;
  return 0;
}

void free_processes(Process* p) { free(p); }

