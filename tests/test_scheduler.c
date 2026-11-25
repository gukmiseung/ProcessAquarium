#include <stdio.h>
#include "scheduler.h"
#include "output.h"

int main(void)
{
    JobInfo jobs[16];
    const char *const argv[] = {"/bin/echo", "hello aquarium", NULL};
    if (scheduler_start(3) < 0) return 1;
    int id = scheduler_add_job(argv, 1);
    printf("add_job result: %d (negative means error/not implemented)\n", id);
    printf("Jobs: %d\n", scheduler_get_jobs(jobs, 16));
    scheduler_stop();
    /* TODO B: add wait, output, ordering, pause/resume and cleanup assertions. */
    return 0;
}
