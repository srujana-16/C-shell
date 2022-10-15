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
#include "header.h"
long long id_init(char *command)
{
    char *tok = command;
    tok = strtok(NULL, " \"\n\t\r");
    long long pid;
    
    if(tok == NULL)
    {
        pid = nul;
        pid = getpid();
    }
    else
        pid = atoi(tok);
    return pid;
}

void print_pinfo(char* tok1, char* tok2, char* executable, long long pid)
{       
    char executable_path[VAL];
    char *temp; 
    printf("pid : %lld\n", pid);
    printf("Process Status : %s", tok1);
    printf("Memory : %s", tok2);
    printf("Executable path : "); 
    long long int r = readlink(executable, executable_path, MAX_BUFFER);
    executable_path[r] = '\0'; 
    temp = strstr(executable_path, HOME);
    if(temp)
    {
        temp += strlen(HOME);
        printf("~%s\n", temp);
    }
    else
        printf("~%s\n", executable_path);
    
}
void pinfo(char *command)
{
    long long pid = id_init(command);
    
    char *executable = malloc(sizeof(char) *sz);
    char *status = malloc(sizeof(char) *(sz));
    sprintf(status,"/proc/%lld/status", pid);
    sprintf(executable,"/proc/%lld/exe", pid);

    FILE *f1 = fopen(status, "r");
    if(f1==NULL)
        printf("Process with ID = %lld does not exist\n", pid);
    
    else
    {
        fclose(f1);
        FILE* file = fopen(status, "r");
        char *status_info = malloc(sizeof(char) *sz);
        char *memory_info = malloc(sizeof(char) *sz);
        if( file!=NULL)
        {
            char info[sz];
            long long int k = 0;
            while (fgets(info, sizeof(info), file)) 
            {
                k++;
                if(k == 3)
                    strcpy(status_info, info);
                else if(k == 18)
                    strcpy(memory_info, info);

            }
            fclose(file);
        }
        else
        {
		    perror("Error in opening the file");
            return;
	    }
        char *tok1 = strtok(status_info, ":\t"); 
        tok1 = strtok(NULL, ":\t");
        char *tok2 = strtok(memory_info, ":\t\r "); 
        tok2 = strtok(NULL, ":\t\r");
        print_pinfo(tok1, tok2, executable, pid);
        free(status_info);
        free(memory_info);
    } 
    free(status);
    free(executable);
}
