/* A: replace this minimal preview with the aquarium UI and controls. */
#include <errno.h>
#include <ncurses.h>
#include "ui.h"
#include "monitor.h"
#include "scheduler.h"
#include "output.h"

int ui_run(void)
{
    if (!initscr()) return -EIO;
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(100);
    for (;;) {
        ProcessInfo processes[16];
        JobInfo jobs[16];
        SystemInfo system;
        int n = monitor_get_processes(processes, 16);
        int j = scheduler_get_jobs(jobs, 16);
        int s = monitor_get_system_info(&system);
        erase();
        mvprintw(0, 0, "Process Aquarium | starter preview | q: quit");
        if (s == 0)
            mvprintw(1, 0, "CPU %.1f%%  MEM %ld/%ld KiB", system.cpu_usage,
                     system.used_memory, system.total_memory);
        else mvprintw(1, 0, "System information: not ready");
        mvprintw(2, 0, "Processes: %d | Jobs: %d", n, j);
        for (int i = 0; i < n && i < 6 && i + 4 < LINES; ++i)
            mvprintw(i + 4, 0, "%d %c %s", (int)processes[i].pid,
                     processes[i].state, processes[i].name);
        if (j > 0 && LINES > 11) {
            OutputLog logs[3];
            int k = output_get_log(jobs[0].job_id, logs, 3);
            mvprintw(10, 0, "Output for job %d:", jobs[0].job_id);
            for (int i = 0; i < k && i + 11 < LINES; ++i)
                mvprintw(i + 11, 0, "%s: %s",
                         logs[i].stream == OUTPUT_STDERR ? "ERR" : "OUT", logs[i].text);
        }
        refresh();
        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;
    }
    endwin();
    return 0;
}
