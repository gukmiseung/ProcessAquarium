/* B: implement pipe/dup2 redirection, nonblocking read, poll and bounded logs. */
#include <errno.h>
#include "output.h"

int output_get_log(int job_id, OutputLog *list, size_t cap)
{
    (void)job_id; (void)list; (void)cap;
    return -ENOSYS;
}
