#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define PA_NAME_MAX 256
#define PA_COMMAND_MAX 512
#define PA_LOG_TEXT_MAX 512

typedef struct {
    pid_t pid, ppid;
    char name[PA_NAME_MAX];
    char state;                 /* Linux /proc state: R,S,D,T,Z,... */
    double cpu_usage;           /* percent of one logical CPU; may exceed 100 */
    long memory_kb;             /* RSS in KiB */
    uint64_t start_time;        /* /proc stat starttime in clock ticks */
} ProcessInfo;

typedef struct {
    double cpu_usage;           /* whole-system percent, 0..100 */
    long total_memory, used_memory; /* KiB */
    long total_swap, used_swap;     /* KiB */
} SystemInfo;

typedef enum {
    JOB_QUEUED, JOB_RUNNING, JOB_PAUSED, JOB_STOPPING,
    JOB_FINISHED, JOB_FAILED, JOB_CANCELED
} JobStatus;

typedef struct {
    int job_id;                 /* positive, unique within this run */
    pid_t pid;                  /* 0 before launch */
    int priority;               /* smaller number = higher priority */
    JobStatus status;
    int exit_code;              /* -1 until normal exit */
    int term_signal;            /* 0 if not terminated by signal */
    char command[PA_COMMAND_MAX];
} JobInfo;

typedef enum { OUTPUT_STDOUT, OUTPUT_STDERR } OutputStream;
typedef struct {
    uint64_t sequence;          /* increasing per job, starts at 1 */
    OutputStream stream;
    char text[PA_LOG_TEXT_MAX];  /* NUL-terminated, may be a partial line */
} OutputLog;
#endif
