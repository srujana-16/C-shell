#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <math.h>
#include <pwd.h>
#include <fcntl.h>
#include "header.h"

void bgprocess_intit(long long int num_argm, char **arg)
{
    arg[num_argm - 1][strlen(arg[num_argm - 1]) - 1] = '\0';
    execute(arg, num_argm, 1);
}

void fgprocess_init(long long int num_argm, char **arg)
{
    arg[num_argm - 1] = NULL;
    execute(arg, num_argm, 1);
}

void removeSpaces(char *com2, long int counter)
{
    long long int k = 0;
    while (k < com2[k])
    {
        if (com2[k] != ' ' && com2[k] != '\t' && com2[k] != '\n')
            com2[counter++] = com2[k];
        k++;
    }
}
void commandExecute(char *command)
{
    char *com1 = (char *)calloc(sizeof(char), VAL);
    char *com2 = (char *)calloc(sizeof(char), VAL);
    strcpy(com1, command);
    strcpy(com2, command);
    long int counter = 0;
    removeSpaces(com2, counter);
    com2[counter] = '\0';
    const char del[] = " \n\t\r";
    com1 = strtok(com1, del);
    long int flag = 0;
    if (com1 == NULL)
        return;

    char **arg = (char **)calloc(sizeof(char *), VAL);
    long int num_argm = 0;
    do
    {
        arg[num_argm] = (char *)malloc(sizeof(char) * strlen(com1) + VAL);
        strcpy(arg[num_argm], com1);
        com1 = strtok(NULL, del);
        num_argm++;
    } while (com1 != NULL);

    if (check_piping(command))
    {
        piping(command);
        return;
    }

    if (initRedirection(command))
    {
        redirection(command);
        return;
    }

    command = strtok(command, " \n\t\r");

    if (strcmp(command, "pwd") == nul)
        pwd();

    else if (!strcmp(arg[nul], "sig"))
        sig(arg, num_argm);

    else if (!strcmp(arg[nul], "bg"))
        bg(arg, num_argm);

    else if (!strcmp(arg[nul], "fg"))
        fg(arg, num_argm);

    else if (strcmp(command, "cd") == nul)
        cd(command);

    else if (strcmp(command, "echo") == nul)
        echo(command);

    else if (strcmp(command, "ls") == nul)
        ls(command);

    else if (strcmp(command, "jobs") == nul)
        jobs_list();

    else if (strcmp(command, "pinfo") == nul)
        pinfo(command);

    else if (!strcmp(arg[num_argm - 1], "&"))
        fgprocess_init(num_argm, arg);

    else if (arg[num_argm - 1][strlen(arg[num_argm - 1]) - 1] == '&')
        bgprocess_intit(num_argm, arg);

    else if (strcmp(command, "history") == 0)
    {
        if (command != NULL)
        {
            command = strtok(NULL, "\t\n ");
        }
        history(command);
    }
    else if ((strcmp(command, "exit") == 0) || (strcmp(command, "quit") == 0))
        exit(0);

    else
        execute(arg, num_argm, 0);

    for (long int j = 0; j < num_argm && arg[j] != NULL; j++)
        free(arg[j]);
    free(com2);
    free(com1);
    free(arg);
}
