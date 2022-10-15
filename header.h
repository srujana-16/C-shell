#ifndef HEADER_H
#define HEADER_H
#define EXIT_FAIL 1
#define MAX_ARGM 400                                                                 
#define nul 0
#define VAL 4000
#define sz 300
#define MAX_BUFFER 10000
#define GREEN "\x1b[92m"
#define BLUE "\x1b[94m"
#define DEF "\x1B[0m"
#define CYAN "\x1b[96m"
#define PINK "\x1b[95m"
#define RED "\x1b[31m"
#define DARKPINK "\x1b[35m"
#define WHITE "\x1b[37m"

char HOSTNAME[VAL], USERNAME[VAL], CWD[VAL], HOME[VAL];
long int x;
char HIST[25][2500];

struct background_job
{
    char jobName[MAX_ARGM];
    pid_t PID;  
};

typedef struct background_job background_job;
background_job jobs[sz];
char CURRENT_JOB[VAL];
int bg_count;
char HistPath[VAL];
char last_command[VAL];
FILE *file;

// Printing the prompt (display requirement)
void printprompt(); 
void Display(char* directory);  

// Builtin commands
void echo(char *command);
void cd_util(char *token);
void error(char* new);
void cd(char *command);
void pwd();

// Execute command 
void commandExecute(char *command);
void removeSpaces(char *com2, long int counter);
void bgprocess_intit(long long int num_argm, char** arg);
void fgprocess_init(long long int num_argm, char** arg);

// ls command 
char* get_filepath(char *DIRECTORY, char *file);
void printls(char* filepath, struct stat Name, char* file);
void ls_out(char *dir_name, long long int flag, long long int a);
void ls_init(char* token, long long fp, long long hidden);
void ls_l(char *directory, char *file);
void ls_call(char* tok);
long long int ls_a(char *file);
void ls(char *command);

// Background and foreground 
void execute(char **argm, int num_argm, int bg);
void signal_cmd(pid_t pid, int stat);
void background(char **argm, pid_t pid, int num_argm);
void foreground(char **argm, pid_t pid, int num_argm, int stat);
void printoutput(pid_t pid, char str[VAL], int f);
void handler(int sig);

// pinfo
long long id_init(char *command);
void print_pinfo(char* tok1, char* tok2, char* executable, long long pid);
void pinfo(char *command);

// history 
void write_history();
void store_history(char* command);
void history(char *command);

// input/output redirection
void redirection(char *command);
void func_redirect(long long int outway, long long int in, char* inF, char* outputF, char** arg);
void errorSignal(long long int outway, char* outputF);
long long int tokenRedirect(char *out, char *command);
long long int initRedirection(char *command);

// pipeline 
int check_piping(char *command);
void piping(char *string);
void checkargs2(int *temp);
void checkargs(int i, int pt, int *temp, int stdout_re, char *args[sz]);

void jobs_list();
void print_list(char *current_stat, int i);

void sig(char **arg, int num_argm);

void bg(char **args, long long int no_args);
void fg(char **args, long long int no_args);

void ctrlc(int sig);
void ctrlz(int signum);
void ctrld();

void chandler(int sig_num);
void zhandler(int sig_num);
void dhandler(int sig_num);


#endif
