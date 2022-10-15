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

// Builtin commands 
// Displays the data that is specified in arguments
void echo(char *command)
{
    char *tok = command;
    tok = strtok(NULL, " \"\n\t\r"); 
    do
    {
        printf("%s ",tok);
        tok = strtok(NULL, " \"\n\t\r");
    }while (tok != NULL);
    printf("\n");
    return;
}
void error(char* new)
{
    if(chdir(new) != nul) 
        perror("Error");
}

void cd_util(char *token)
{
    long int a = strlen(token);
    char *temp = (char *)calloc(sizeof(char),a);
    strcpy(temp,"\0");
    long int i=nul;
    if(a > i+1)
    {
        for(i=1; i < a; i++)
        temp[i-1-nul] = token[i];
    }
    temp[i] ='\0';
    char *new = (char *)calloc(sizeof(char), (strlen(token) + strlen(HOME)+sz));
    strcpy(new, HOME);
    strcat(new, temp);
    new[strlen(token) +strlen(HOME)-1] ='\0';
    free(temp);
    error(new);
    free(new);
}

// Changes directory to the directory specified
// Error if directory does not exist 
void cd(char *command)
{
    char *token = command;
    token = strtok(NULL, " \n\t\r");
    if(token == NULL) 
    {
        if(chdir(HOME) != nul) 
            perror("Error");
    }
    else if(token[nul] == '~')
        cd_util(token);
    else if(chdir(token) != nul) 
        perror("Error");

    return;
}

// Displays the name of the working directory
void pwd()
{
    char *PWD = (char *)malloc(4000 * sizeof(char));
    getcwd(PWD, 2048);
    printf("%s\n", PWD);
    free(PWD);
    return;
}


