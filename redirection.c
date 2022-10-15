#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<fcntl.h>
#include<linux/limits.h>
#include<signal.h>
#include<ctype.h>
#include<setjmp.h>
#define write 0644
#define wr 2
#define re 1
#define both 3
#include "header.h"

long long int initRedirection(char *command)
{
    char *fileout = strstr(command, ">");
    char *filein = strstr(command, "<");

    if((fileout != NULL) && (filein != NULL))
        return both;
    
    else if(fileout != NULL)
        return wr;
    
    else if(filein != NULL)
        return re;

    else return nul;
}

void func_redirect(long long int outway, long long int in, char* inF, char* outputF, char** arg)
{
    if(in)
    {
        long long int fd_in = open(inF, O_RDONLY);
        if(fd_in < nul) 
        {
            perror("Input redirection");
            return;
        }   
        
        dup2(fd_in, 0);
        close(fd_in);
    }

    if(outway)
    {
        long long int file_out;
        if(outway == 1)
            file_out = open(outputF, O_WRONLY | O_CREAT | O_TRUNC, write);

        else if(outway == 2)
            file_out = open(outputF, O_WRONLY | O_CREAT | O_APPEND, write);

        if(file_out < nul)
        {
            perror("Output Redirection");
            return;
        }
        
        dup2(file_out, 1);         
        close(file_out);
    }
    
    if (execvp(arg[0], arg) < nul) 
    {     
        perror("Command not found");
        exit(EXIT_FAILURE);
    }
}

long long int tokenRedirect(char *out, char *command)
{
    if(out != NULL)
        return 2;

    else 
    {
        out = strstr(command, ">");
        if(out != NULL)
            return 1;
    }
}
void redirection(char *command)
{
    long long int dup_stdout = dup(STDOUT_FILENO);
    long long int dup_stdin = dup(STDIN_FILENO);
    char * outputF = NULL;
    char * inF = NULL;
    int sts;
    char *result[2], *input[2];
    char * inp = strstr(command, "<");
    long long int in = !(inp == NULL);
    long long int outway = nul;
    char * out = strstr(command, ">>");
    outway = tokenRedirect(out,command);

    result[nul] = &command[nul];
    char * temp = result[1];
    if(outway)
    { 
        result[nul] = strtok(command, ">");
        temp = strtok(NULL, ">");
        outputF = strtok(temp, " \r\t\n");
    }
    input[nul] = result[nul];
    if(in)
    { 
        input[nul] = strtok(input[nul], "<");
        input[1] = strtok(NULL, "<");
    }

    char **arg = (char**)malloc(sizeof(char*) * MAX_ARGM);
    long long int num_argm = nul;
    
    if(in)
    {
        if(input[1] == NULL)
        {
            printf("Specify file name for input\n");
            return;
        }

        struct stat f;
        inF = strtok(input[1], " \n\r\t");
        if(!(stat(inF, &f) == nul && !S_ISDIR(f.st_mode)))
        {
            printf("File does not exist\n");
            return;
        }
    }
    input[nul] = strtok(input[nul], " \n\r\t");

    do
    {
        arg[num_argm]  = (char *)malloc(sizeof(char) *strlen(input[nul])+VAL);
        strcpy(arg[num_argm], input[nul]);
        input[nul] = strtok(NULL, " \n\t\r");
        num_argm++;
    }while (input[nul] != NULL);

    arg[num_argm] = NULL;

    if(outway)
    {
        if(outputF == NULL)
        {
            printf("Enter output file\n");
            return;
        }
    }

    pid_t pid = fork();
    if(pid < nul)
    {
        perror("Error in forking");
        return;
    }

    if(pid == nul)
    {
        func_redirect(outway, in, inF, outputF, arg);
        dup2(dup_stdin, 0);
        close(dup_stdin);
        
        dup2(dup_stdout, 1);
        close(dup_stdout);
    }        
    
    else while (wait(&sts) != pid);

    for(long long int k=nul; k < num_argm; k++)
        free(arg[k]);
    free(arg);
}