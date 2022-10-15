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
#define init 1
#define GREEN "\x1b[92m"
#define BLUE "\x1b[94m"
#define DEF "\x1B[0m"
#define CYAN "\x1b[96m"
#define PINK "\x1b[95m"
#define RED "\x1b[31m"
#define DARKPINK "\x1b[35m"
#define WHITE "\x1b[37m"

long long int ls_a(char *file)
{
    if(file[nul] == '.')
        return 1;
    else 
        return 0;
}

char* get_filepath(char *DIRECTORY, char *file)
{
    long long n = strlen(DIRECTORY);
    char *path = (char *)malloc(sizeof(char) * (n + strlen(file)+VAL));
    strcpy(path, DIRECTORY);
    strcat(path, "/");
    strcat(path, file);
    return path;
}

void printls(char* filepath, struct stat Name, char* file)
{
    struct group *gr = getgrgid(Name.st_gid);
    struct passwd *pass =  getpwuid(Name.st_uid);
    char *printperm = malloc(sizeof(char) * VAL);
    printperm[nul] = (stat(filepath, &Name) == nul && S_ISDIR(Name.st_mode)) ? 'd' : '-';
    printperm[init] = (Name.st_mode& S_IRUSR) ? 'r' : '-';
    printperm[2] = (Name.st_mode& S_IWUSR) ? 'w' : '-';
    printperm[3] = (Name.st_mode& S_IXUSR) ? 'x' : '-';
    printperm[4] = (Name.st_mode& S_IRGRP) ? 'r' : '-';
    printperm[5] = (Name.st_mode& S_IWGRP) ? 'w' : '-';
    printperm[6] = (Name.st_mode& S_IXGRP) ? 'x' : '-';
    printperm[7] = (Name.st_mode& S_IROTH) ? 'r' : '-';
    printperm[8] = (Name.st_mode& S_IWOTH) ? 'w' : '-';
    printperm[9] = (Name.st_mode& S_IXOTH) ? 'x' : '-';
    printperm[10] = '\0';

    char *time = (char *)malloc(sizeof(char) * VAL); 
    strftime(time, 50, "%b  %d %H:%M", localtime( &Name.st_mtime));
    if(printperm[nul] == 'd')
        printf("%s\t%ld\t%s\t%s\t%ld\t %s\t\x1B[1;34m%s\x1B[0m\n", printperm, Name.st_nlink, pass->pw_name,gr->gr_name, Name.st_size, time, file);
    else if(printperm[3] == 'x')
        printf("%s\t%ld\t%s\t%s\t%ld\t %s\t\x1B[1;32m%s\x1B[0m\n", printperm, Name.st_nlink, pass->pw_name,gr->gr_name, Name.st_size, time, file);
    else 
        printf("%s\t%ld\t%s\t%s\t%ld\t %s\t%s\n", printperm, Name.st_nlink, pass->pw_name,gr->gr_name, Name.st_size, time, file);
    free(printperm);
    free(filepath);
}

void ls_l(char *DIRECTORY, char *file)
{
    char* filepath = get_filepath(DIRECTORY, file);

    struct stat Name;
    if(stat(filepath, &Name) < nul)
    {
        perror("Error");
        return;
    }
    char *printperm = malloc(sizeof(char) * VAL);
    printls(filepath, Name, file);
}

void ls_out(char *dir_name, long long int flag, long long int hidden)
{
    struct dirent **file;
    long long int n = scandir(dir_name, &file, NULL, alphasort);
    if(n < nul)
    {
        perror("Error");
        return;
    }
    if(flag==nul)
    {
        for(long long int i=nul; i<n; i++) 
        { 
            if(hidden == nul && !ls_a(file[i]->d_name) || hidden == init)
            {
                free(file[i]);
                printf("%s\t",file[i]->d_name);  
            }
        }
        printf("\n");
        free(file);
    }
    else if(flag==init)
    {
        for(long long int i=nul; i<n; i++)
        {
            if(hidden == nul && !ls_a(file[i]->d_name) || hidden == init)
                ls_l(dir_name, file[i]->d_name);
            free(file[i]);
        }
        free(file);
    }
}

void ls_init(char* tok, long long fp, long long hidden)
{
    char *filepath;
    long long int n = strlen(tok);
    char *temp= (char *)malloc(sizeof(char) *n +1-nul);
    temp[n] ='\0';
    if(tok[nul] == '~')
    {
        if(n > 1)
        {
            for(long long int i=init; i < n; i++)
                temp[i-1] = tok[i];
        }
                
        filepath = (char *)malloc(sizeof(char) *(n + strlen(HOME) + VAL));
        strcpy(filepath, HOME);
        strcat(filepath, temp);
        filepath[n + strlen(HOME)-1-nul] = '\0';
        free(temp);
    }
    else 
    {
        filepath = (char *)malloc(sizeof(char)*n + VAL);
        strcpy(filepath, tok);
    }   
    ls_out(filepath, fp, hidden);
    free(filepath);
}

void ls(char *command)
{
    long long int flag = nul, hidden =nul, x = nul, y = nul;
    char *tok = command;

    tok = strtok(NULL, " \n\t\r");

    while (tok != NULL)
    {
        x = init;
        if(!strcmp(tok,"-l"))
            flag = init;
    
        else if(!strcmp(tok,"-a"))
            hidden = init;

        else if(!strcmp(tok,"-al") || !strcmp(tok,"-la"))
            flag = init, hidden = init;

        else
        {
            y = init;
            ls_init(tok,flag ,hidden);
        }
        tok = strtok(NULL, " \n\t\r");
    }
    if(x==nul || (x==1 && y == nul))
        ls_out(".", flag, hidden);
        
}