#include <stdio.h>
#include "monitor.h"
#include "scheduler.h"
#include "ui.h"

int main(void)
{
    int rc = monitor_start();
    if (rc < 0) {
        fprintf(stderr, "monitor_start failed: %d\n", rc);
        return 1;
    }
    rc = scheduler_start(3);
    if (rc < 0) {
        monitor_stop();
        fprintf(stderr, "scheduler_start failed: %d\n", rc);
        return 1;
    }
    rc = ui_run();
    scheduler_stop();
    monitor_stop();
    return rc < 0 ? 1 : 0;
}
