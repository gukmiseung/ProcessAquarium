/* B: implement job queue, worker lifecycle, spawn/reap and signal control. */
#include <errno.h>
#include "scheduler.h"

int scheduler_start(size_t max_running)
{
    return max_running == 0 ? -EINVAL : 0;
}
void scheduler_stop(void) {}
int scheduler_add_job(const char *const argv[], int priority)
{
    (void)argv; (void)priority;
    return -ENOSYS;
}
int scheduler_pause_job(int job_id) { (void)job_id; return -ENOSYS; }
int scheduler_resume_job(int job_id) { (void)job_id; return -ENOSYS; }
int scheduler_stop_job(int job_id) { (void)job_id; return -ENOSYS; }
int scheduler_get_jobs(JobInfo *list, size_t cap)
{
    (void)list; (void)cap;
    return 0;
}
