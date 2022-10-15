#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <time.h>
#include <math.h>
#include <pwd.h>
#include <fcntl.h> 
#include <sys/wait.h>
#include "header.h"
pid_t shellID,childID;
char **tokenizer(char *input)
{
    char *token = strtok (input, ";");
    char **temp_array = (char**)calloc(MAX_ARGM, sizeof(char *));
    x = nul;
    do
    {
        temp_array[x++] = token;
        token = strtok (NULL, ";");
    }
    while(token!=NULL);
    return temp_array;
}

void p_init()
{
    gethostname(HOSTNAME, sizeof(HOSTNAME));      // Get host name
    getlogin_r(USERNAME, sizeof(USERNAME));       // Get username 
    pid_t p_id = getpid();
    shellID = p_id;
    char *exe = malloc(sizeof(char) *(80));
    sprintf(exe,"/proc/%d/exe", p_id);
    long int ret = readlink(exe, HOME, 2000);
    HOME[ret] = '\0';
    long int k = strlen(HOME)-1;
    while(k>=nul)
    {
        if(HOME[k] == '/')
        {
            HOME[k] = '\0';
            break;
        }
        k--;
    }
}

char *takeInput()
{
    ssize_t in_size = nul;
    char *input = NULL;
    getline(&input, &in_size, stdin);
    return input;
}

void addHistory(char *command)
{
    if(strcmp(last_command,command) && strcmp(command,"\n"))
    {
        strcpy(last_command,command);
        write_history(last_command);
    }
}

int main()
{
    printf("\033[0;36m\n      Welcome to C-Shell :) \n\n\033[0m");
    p_init();
    char *in;
    signal(SIGINT, dhandler); 
    signal(SIGQUIT, dhandler); 
    signal(SIGTSTP, zhandler); 
    signal(SIGCHLD, handler); 
    strcpy(HistPath,HOME);
    strcat(HistPath,"/hist.txt"); 
    strcpy(last_command,"");
    while(1)
    {
        pid_t childID = -1;
        printprompt();
        in = takeInput();
        char **comm = tokenizer(in);
        char history_input[1024] = "";
        strcpy(history_input,in);
        addHistory(history_input);
        for(long int i = 0; i < x; i++)
            commandExecute(comm[i]);
    } 
    return 0;
}