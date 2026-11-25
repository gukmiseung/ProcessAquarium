/* C: replace the placeholder with /proc collection and a pthread worker. */
#include <errno.h>
#include "monitor.h"

int monitor_start(void) { return 0; }
void monitor_stop(void) {}
int monitor_get_processes(ProcessInfo *list, size_t cap)
{
    (void)list; (void)cap;
    return 0;
}
int monitor_get_system_info(SystemInfo *info)
{
    (void)info;
    return -ENOSYS;
}
