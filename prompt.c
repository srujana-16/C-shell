#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <grp.h>
#include <unistd.h>
#include "header.h"

// Display requirement 
// Prints the prompt with username, hostname and working directory
void Display(char* directory)
{
    printf("<\x1B[1;32m%s@%s\x1B[0m:\x1B[0;34m%s\x1B[0m>", USERNAME, HOSTNAME, directory);
}

void printprompt()
{
    char *directory;
    if (getcwd(CWD, sizeof(CWD)) == NULL)
    {
       perror("Error : getcwd() error");
       exit(EXIT_FAIL);
    }
    else
    {
        char *p = strstr(CWD, HOME);
        if(p!=NULL)
        {
            p+=strlen(HOME)-1;
            p[0]= '~';
            directory = p;
            Display(directory);
        }
        else
        {
            p = CWD;
            directory = p;
            Display(directory);
        }
    }
}