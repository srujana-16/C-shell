## To compile and run the code with cmake do the following:

- Download the file and cd to '/2020102005_osn_A2' 
- Run 'make' 
- Run ./srujshell to get a prompt of the form username@system_name:curr_dir
- In order to exit, you can run either exit or quit.

## Implemented features: 

1. pwd - Displays the name of current working directory.
2. cd [directory] - Changes current working directory to the directory specified
3. echo [argument] - Displays whatever is specified in [argument]
4. ls - Lists all the files and directories in the specified directory
5. pinfo [pid] - Displays process info such as status, memory and executable path about given pid. If no pid is mentioned, displays information of the shell. 
6. discover - Searches for files in a directory hierarchy
7. jobs - Prints a list of all currently running jobs along with their PID in order of their creation and also their status.
8. sig <jobnumber> <signalnumber> - Takes the job number (assigned by your shell) of a running job and sends the signal corresponding to the
signal number to that process. 
9. fg <jobNumber> - Brings the running or stopped background job corresponding to job number to the foreground, and changes
its state to running. 
10. bg <jobNumber> -  Changes the state of a stopped background job to running (in the background).
11. Background and foreground processes - Shell supports '&' operator which lets a program run in background and displays a proper message when it exits. 
12. history - Prints history
13. ​CTRL-Z - Pushes any currently running foreground job into the background, and change its state from
‘running’ to ‘stopped.
14. CTRL-C - Interrupts any currently running foreground job, by sending it the SIGINT signal.
15. CTRL-D - Logs you out of the shell.
16. Autocompletion - Autocompletes the file name on pressing the tab key. 
