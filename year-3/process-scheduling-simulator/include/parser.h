#pragma once

#include "models.h"

int load_processes(const char* path, Process** out, int* out_count);
void free_processes(Process* p);

