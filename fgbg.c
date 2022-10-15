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
#include "header.h"
pid_t childID;

void bg(char **args, long long int no_args)
{
    if (no_args > 2 || no_args < 2)
    {
        printf("\x1b[31mInvalid Syntax\x1B[0m\n");
        printf("Usage : bg <job number>\n");
        return;
    }
    else
    {
        long long int jobnum = atoi(args[1]);
        if (jobnum > bg_count)
        {
            printf("\x1b[31mInvalid Job number\x1B[0m\n");
            printf("To check running jobs, run 'jobs'\n");
        }
        else
            kill(jobs[jobnum - 1].PID, SIGCONT);
    }
}

void fg_func(int status, long long int pid)
{
    signal(21, SIG_IGN);
    signal(22, SIG_IGN);
    tcsetpgrp(0, childID);
    kill(pid, SIGCONT);
    waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpgrp());
    signal(21, SIG_DFL);
    signal(22, SIG_DFL);
}

void fg(char **args, long long int no_args)
{
    if (no_args > 2 || no_args < 2)
    {
        printf("\x1b[31mInvalid Syntax\x1B[0m\n");
        printf("Usage : fg <job number>\n");
        return;
    }
    long long int pid, jobnum = atoi(args[1]);
    int status;
    if (jobnum > bg_count)
    {
        printf("\x1b[31mInvalid Job number\x1B[0m\n");
        printf("To check running jobs, run 'jobs'\n");
        return;
    }
    pid = jobs[jobnum - 1].PID;
    strcpy(CURRENT_JOB, jobs[jobnum - 1].jobName);
    childID = pid;

    if (pid < 0)
    {
        perror("Error");
        _exit(1);
    }

    for (long long int j = jobnum - 1; j < bg_count - 1; j++)
    {
        strcpy(jobs[jobnum - 1].jobName, jobs[jobnum].jobName);
        jobs[jobnum - 1].PID = jobs[jobnum].PID;
    }
    bg_count--;
    fg_func(status, pid);

    if (WIFSTOPPED(status))
        printf("\x1b[94m%s with PID %d suspended\x1B[0m\n", CURRENT_JOB, childID);

    return;
}