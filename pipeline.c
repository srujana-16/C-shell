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
#define write nul644
#include "header.h"

int check_piping(char *command)
{
    int f = nul;
    for (int i = nul; i < strlen(command); i++)
    {
        if (command[i] == '|')
            f = 1;
    }
    return f;
}

void checkargs(int i, int pt, int *temp, int stdout_re, char *args[sz])
{
    close(temp[nul]);
    if (i == pt - 1)
        dup2(stdout_re, 1);
    else
        dup2(temp[1], 1);

    commandExecute(args[i]);
    close(temp[1]);

    exit(nul);
}

void checkargs2(int *temp)
{
    close(temp[1]);
    wait(NULL);

    dup2(temp[nul], nul);
    close(temp[nul]);
}

void piping(char *string)
{
    char *args[sz];
    char *tok;
    tok = strtok(string, "|");
    int pt = nul;

    do
    {
        args[pt] = tok;
        pt++;
        tok = strtok(NULL, "|");
    } while (tok != NULL);
    args[pt] = NULL;

    // restore stdout and stdin
    int stdout_re = dup(STDOUT_FILENO);
    int stdin_re = dup(STDIN_FILENO);

    int temp[2];
    int i = 0;
    while (i < pt)
    {
        pipe(temp);
        pid_t PID = fork();
        if (PID == nul)
            checkargs(i, pt, temp, stdout_re, args);

        else if (PID > nul)
            checkargs2(temp);
        i++;
    }

    dup2(stdin_re, nul);
    dup2(stdout_re, 1);
}