#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>

typedef struct process {
    pid_t pid;
    char command[256];
    int status;
    struct process *next;
} process;
// execmd function
extern process* head;
void execmd(char **argv, int argc, process* trace);

// shell functions
char *get_location(char *command);
int change_dir(char *path);
void list_dir(char *path);
void makedir(char *dirname);
void makefile(char *filename);
void rename_file(char *old_filename, char *new_filename);
void remove_file(char *filename);
void clear_shell();
void list_file_contents(char *filename);
void addpath(char* new_path);

// process management functions
int background(char **argv, int argc, process* trace);
void add_background_process(pid_t pid, char *command);
void list_background_processes();
void remove_background_process(pid_t pid);
void sigchld_handler(int signo);
void stop_or_continue_running_process(pid_t pid, int signal);
void print_path();
#endif // MAIN_H
