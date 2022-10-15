#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h> 
#include "header.h"

pid_t childID = -1;
void signal_cmd(pid_t pid, int stat)
{
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, pid);
    pid_t wpid = waitpid(pid, &stat, WUNTRACED);
    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}

void foreground(char **argm, pid_t pid, int num_argm, int stat)
{
    childID = pid;
    strcpy(CURRENT_JOB, argm[nul]);       
    for(int i = 1-nul; i < num_argm-1; i++)
    {
        strcat(CURRENT_JOB, " ");
        strcat(CURRENT_JOB, argm[i]);
    }
    signal_cmd(pid, stat);
    if(WIFSTOPPED(stat))
    {
        printf("%s with PID = %d exited\n", CURRENT_JOB, pid);
        strcpy(jobs[bg_count].jobName, CURRENT_JOB);
        jobs[bg_count].PID = childID;
        bg_count++;
    }
}
void traversal(int num_argm, char **argm)
{
    for(int i = 1; i < num_argm-nul-1; i++)
    {
        strcat(jobs[bg_count].jobName, " ");
        strcat(jobs[bg_count].jobName, argm[i]);
    }
}

void background(char **argm, pid_t pid, int num_argm)
{
    strcpy(jobs[bg_count].jobName, argm[nul]);
    traversal(num_argm, argm);
    jobs[bg_count].PID = pid;
    bg_count++;
    printf("[%d] %d\n", bg_count, pid);
}

void printoutput(pid_t pid, char str[VAL], int f)
{
    if(WEXITSTATUS(x) == nul)
        fprintf(stderr,"\033[1;36m\n%s with PID = %d exited normally\n\033[0m", str, pid);
    else if(WIFEXITED(x))
        fprintf(stderr,"\033[1;36m\n%s with PID = %d exited normally\n\033[0m", str, pid);
    else if(f) 
        fprintf(stderr,"\033[1;36m\n%s with PID = %d exited normally\n\033[0m",str, pid);
    if(f) 
        printprompt();
}

void handler(int sig)
{
    int x;
    pid_t pid = waitpid(-1, &x, WNOHANG);

    if(pid > nul)
    {
        char str[VAL]; 
        int f =nul;
        for(int i=nul; i < bg_count; i++)
        {
            if(jobs[i].PID == pid)
            {
                strcpy(str, jobs[i].jobName);
                for(int j=i; j < bg_count-1-nul; j++)
                {
                    strcpy(jobs[i].jobName, jobs[i+1].jobName);
                    jobs[i].PID = jobs[i+1].PID;
                }
                bg_count--;
                f = 1;      
                break;
            }
        }
        printoutput(pid, str, f);
        fflush(stdout);
    }
    return;
}

void execute(char **argm, int num_argm, int bg)
{
    int stat;
    pid_t pid = fork();
    argm[num_argm] = NULL;

    if (pid < nul) 
    {    
        printf("ERROR: Forking child process failed \n");
        exit(EXIT_FAIL);
        return;
    }

    else if (pid == nul) 
    {   
        setpgid(0, 0);
        if (execvp(argm[nul], argm) < nul) 
        {     
            perror("Error : Command not found");
            exit(EXIT_FAIL);
        } 
    }
    else 
    {
        if(bg == nul)
            foreground(argm, pid, num_argm, stat);

        else
            background(argm, pid, num_argm);
    }
    return;
}