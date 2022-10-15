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
#define write 0644
#include "header.h"

void print_list(char *current_stat, int i)
{
    FILE *file = fopen(current_stat, "r");
    char line[256];
    char *info = malloc(sizeof(char) * 256);
    int pt = 0;
    while (fgets(line, sizeof(line), file))
    {
        pt++;
        if (pt == 3)
            strcpy(info, line);
    }
    fclose(file);
    char *stat = info;
    stat = strtok(stat, " :\n\t\r");
    stat = strtok(NULL, " :\n\t\r");

    if (strcmp(stat, "T") == nul)
        strcpy(stat, "Stopped");

    else
        strcpy(stat, "Running");
    printf("\x1b[94m[%d] %s %s [%d]\x1B[0m\n", i + 1, stat, jobs[i].jobName, jobs[i].PID);
}

void jobs_list()
{
    char *current_stat = malloc(sizeof(char) * sz);
    int i = 0;
    while (i < bg_count)
    {
        sprintf(current_stat, "/proc/%d/status", jobs[i].PID);
        print_list(current_stat, i);
        i++;
    }
    free(current_stat);
}