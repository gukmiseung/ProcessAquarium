#ifndef OUTPUT_H
#define OUTPUT_H
#include "common.h"

/* Thread-safe copy of up to cap most recent retained records, oldest first.
 * Return count (0 if empty), or negative errno. Owned jobs only.
 * Scheduler owns output worker lifecycle; private helpers may be added by B. */
int output_get_log(int job_id, OutputLog *list, size_t cap);
#endif
