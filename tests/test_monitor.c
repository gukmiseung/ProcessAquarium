#include <stdio.h>
#include <time.h>
#include "monitor.h"

int main(void)
{
    ProcessInfo list[32];
    SystemInfo info;
    if (monitor_start() < 0) return 1;
    struct timespec delay = {1, 0};
    nanosleep(&delay, NULL);
    int n = monitor_get_processes(list, 32);
    int s = monitor_get_system_info(&info);
    printf("Processes: %d, system info result: %d\n", n, s);
    for (int i = 0; i < n; ++i)
        printf("%d %s %c %.1f%%\n", (int)list[i].pid, list[i].name,
               list[i].state, list[i].cpu_usage);
    monitor_stop();
    /* TODO C: add assertions after implementing actual collection. */
    return n < 0;
}
