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
#include "header.h"
pid_t childID, shellID;

void chandler(int sig_num){}

void zhandler(int sig_num){
    return;
}

void dhandler(int sig_num){
    exit(0);
}
