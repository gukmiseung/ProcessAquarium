/* A: fixtures only; never linked into production. */
#include <errno.h>
#include "monitor.h"
#include "scheduler.h"
#include "output.h"

int monitor_start(void) { return 0; }
void monitor_stop(void) {}
int monitor_get_processes(ProcessInfo *list, size_t cap)
{
    const ProcessInfo demo[] = {
        {2154, 1, "demo-parent", 'R', 12.3, 54000, 10},
        {3101, 2154, "demo-child", 'S', 2.1, 82000, 20},
        {567, 2154, "demo-paused", 'T', 0.0, 32000, 30}
    };
    size_t n = cap < 3 ? cap : 3;
    for (size_t i = 0; i < n; ++i) list[i] = demo[i];
    return (int)n;
}
int monitor_get_system_info(SystemInfo *info)
{
    *info = (SystemInfo){23.7, 8000000, 2000000, 2000000, 100000};
    return 0;
}
int scheduler_start(size_t max_running) { (void)max_running; return 0; }
void scheduler_stop(void) {}
int scheduler_add_job(const char *const argv[], int priority)
{ (void)argv; (void)priority; return -ENOSYS; }
int scheduler_pause_job(int id) { (void)id; return -ENOSYS; }
int scheduler_resume_job(int id) { (void)id; return -ENOSYS; }
int scheduler_stop_job(int id) { (void)id; return -ENOSYS; }
int scheduler_get_jobs(JobInfo *list, size_t cap)
{
    if (!cap) return 0;
    list[0] = (JobInfo){1, 2154, 1, JOB_RUNNING, -1, 0, "demo"};
    return 1;
}
int output_get_log(int job_id, OutputLog *list, size_t cap)
{
    if (job_id != 1) return -ENOENT;
    const OutputLog demo[] = {{1, OUTPUT_STDOUT, "Processing..."}, {2, OUTPUT_STDERR, "Demo warning"}};
    size_t n = cap < 2 ? cap : 2;
    for (size_t i = 0; i < n; ++i) list[i] = demo[i];
    return (int)n;
}
