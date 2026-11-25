#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "common.h"

/* Start/stop called by main. Stop joins workers and cleans up owned children. */
int scheduler_start(size_t max_running);
void scheduler_stop(void);
/* argv is NULL-terminated and deep-copied before returning. No shell expansion.
 * Return positive job ID, or negative errno. */
int scheduler_add_job(const char *const argv[], int priority);
/* Commands target owned jobs by job_id, never arbitrary PIDs.
 * Return 0 if accepted, negative errno otherwise. */
int scheduler_pause_job(int job_id);
int scheduler_resume_job(int job_id);
int scheduler_stop_job(int job_id);
/* Include queued and completed jobs; copy at most cap, return count or -errno. */
int scheduler_get_jobs(JobInfo *list, size_t cap);
#endif
