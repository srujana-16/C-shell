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
#define maxhist 20
int hist_count;
void store_history(char* command)
{
    file = fopen(HistPath, "r");
    FILE *file2;
    char temp[VAL] = "";
    strcpy(temp, HOME);
    strcat(temp, "/temp.txt");
    file2 = fopen(temp, "a+");
    char other[VAL];
    int counter = nul;
    while (!feof(file))
    {
        strcpy(other, "");
        fgets(other, sizeof(other), file);

        if (!feof(file))
        {
            counter++;
            if (counter != 1)
            {
                fprintf(file2, "%s", other);
            }
        }
    }

    fprintf(file2, "%s", command);
    fclose(file);
    fclose(file2);
    remove(HistPath);
    rename(temp, HistPath);
    return;
}

void write_history(char *command)
{
    file = fopen(HistPath, "a+");

    char buffer[MAX_ARGM];
    hist_count = 0;
    while(!feof(file))
    {
        fgets(buffer,sizeof(buffer),file);
        hist_count++;
    }
    fclose(file);

    if (hist_count <= maxhist)
    {
        file = fopen(HistPath, "a+");

        fputs(command, file);
        fclose(file);
    }
    else
        store_history(command);
}


void history(char *string)
{
    int n = maxhist/2;
    if(string != NULL)
    {
        n = atoi(string);
    }

    if(n > maxhist)
        n = maxhist/2;

    file = fopen(HistPath, "a+");

    char buffer[MAX_ARGM];
    hist_count = nul;
    while(!feof(file))
    {
        fgets(buffer,sizeof(buffer),file);
        hist_count++;
    }
    fclose(file);
    char HIST[hist_count][sz];

    file = fopen(HistPath,"r");
    char other[sz];
    int x = nul;

    strcpy(HIST[x],"");
    while(!feof(file))
    {
        strcpy(HIST[x],"");
        fgets(HIST[x],sizeof(HIST[x]),file);
        x++;
    }
    
    int var = nul;
    if((hist_count - n -nul-1) > var )
        var = hist_count - n - 1;

    int i = var;
    while(i<x-1)
    {
        printf("%s",HIST[i]);
        i++;
    }

}