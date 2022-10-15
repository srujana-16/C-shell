#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <signal.h>
#include <ctype.h>
#include <setjmp.h>
#define BLUE "\x1b[94m"
#define RED "\x1b[31m"
#include "header.h"

void sig(char **arg, int num_argm)
{
    if (num_argm < 3 || num_argm > 3)
    {
        printf("\x1b[31mInvalid Syntax\x1B[0m\n");
        printf("Usage : sig <job number> <signal number>\n");
        return;
    }
    
    int jobnum = atoi(arg[1]);
    int sig = atoi(arg[2]);
    if (jobnum > bg_count)
    {
        printf("\x1b[31mInvalid Job number\x1B[0m\n");
        printf("To check running jobs, run 'jobs'\n");
        return;
    }
    kill(jobs[jobnum - 1].PID, sig);
}