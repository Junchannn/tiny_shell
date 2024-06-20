#include "main.h"

void execmd(char **argv, int argc, process* trace) {
    
    if (argc == 0) return;

    if (!strcmp(argv[0], "ls")) {
        if (argc == 1)
            list_dir(".");
        else
            list_dir(argv[1]);
    } 
    
    else if (!strcmp(argv[0], "clear")) {
        clear_shell();
    } 
    
    else if (!strcmp(argv[0], "mkdir")) {
        if (argc > 1) {
            makedir(argv[1]);
        } else {
            fprintf(stderr, "mkdir: missing operand\n");
        }
    } 
    
    else if (!strcmp(argv[0], "touch")) {
        if (argc > 1) {
            makefile(argv[1]);
        } else {
            fprintf(stderr, "touch: missing operand\n");
        }
    } 
    
    else if (!strcmp(argv[0], "mv")) {
        if (argc > 2) {
            rename_file(argv[1], argv[2]);
        } else {
            fprintf(stderr, "mv: missing operand\n");
        }
    } 
    
    else if (!strcmp(argv[0], "ps")) {
        list_background_processes();
    } 

    else if (!strcmp(argv[0], "kill")) {
        if (argc < 2){
            fprintf(stderr, "kill: missing operand\n");
        } else if (argc > 2){
            fprintf(stderr, "kill: wrong !!!\n");
        } else {
            remove_background_process(atoi(argv[1]));
        }
    } 
    
    else if (!strcmp(argv[0],"continue")){
        if (argc < 2){
            fprintf(stderr, "continue: missing operand\n");
        } else if (argc > 2){
            fprintf(stderr, "continue: wrong !!!\n");
        } else {
            stop_or_continue_running_process(atoi(argv[1]), SIGCONT);
        }
    }
    else if (!strcmp(argv[0],"stop")){
        if (argc < 2){
            fprintf(stderr, "stop: missing operand\n");
        } else if (argc > 2){
            fprintf(stderr, "stop: wrong !!!\n");
        } else {
            stop_or_continue_running_process(atoi(argv[1]), SIGSTOP);
        }
    } 
    
    else if (!strcmp(argv[0],"bash")){
        execvp(argv[0], argv);
        perror("Execution error\n");
        exit(EXIT_FAILURE);
    }

    else if (!strcmp(argv[0],"cat")){
        if (argc < 2){
            fprintf(stderr, "cat: missing operand\n");
        } else if (argc > 2){
            fprintf(stderr, "cat: wrong !!!\n");
        } else {
            list_file_contents(argv[1]);
        }
    }

    else if (!strcmp(argv[0], "path")){
        if (argc != 1){
            fprintf(stderr, "path: wrong !!!\n");
        }
        else print_path();
    }
    
    else if (!strcmp(argv[0], "addpath")){
        if (argc != 2){
            fprintf(stderr, "addpath: wrong !!!\n");
        }
        else addpath(argv[1]);
        
    }

    else{
        execvp(argv[0],argv);
    }
}
