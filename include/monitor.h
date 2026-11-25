#ifndef MONITOR_H
#define MONITOR_H
#include "common.h"

/* Lifecycle called by main only. 0 success, negative errno on failure. */
int monitor_start(void);
void monitor_stop(void);
/* Thread-safe copies into caller-owned buffers; return count or negative errno.
 * cap is the maximum number of records; truncated snapshots are allowed. */
int monitor_get_processes(ProcessInfo *list, size_t cap);
int monitor_get_system_info(SystemInfo *info);
#endif
