#include "main.h"


void add_background_process(pid_t pid, char *command) {
    process *new_process = (process *)malloc(sizeof(process));
    new_process->pid = pid;
    new_process->status = 1; // Assuming process starts as running
    strncpy(new_process->command, command, sizeof(new_process->command) - 1);
    new_process->command[sizeof(new_process->command) - 1] = '\0';
    new_process->next = head;
    head = new_process;
}

// Function to list all background processes
void list_background_processes() {
    process *current = head;
    printf("Background processes:\n");
    while (current != NULL) {
        printf("PID: %d, Command: %s\n", current->pid, current->command);
        current = current->next;
    }
}

// Function to remove a background process from the list
void remove_background_process(pid_t pid) {
    process **current = &head;
    while (*current != NULL) {
        if ((*current)->pid == pid) {
            process *to_free = *current;
            *current = (*current)->next;
            kill(pid, SIGKILL); // Send SIGKILL to ensure the process is terminated
            free(to_free);
            return;
        }
        current = &((*current)->next);
    }
    fprintf(stderr, "No such process with PID %d\n", pid);
}

// Function to stop or continue a running process
void stop_or_continue_running_process(pid_t pid, int signal) {
    process *current = head;
    while (current != NULL) {
        if (current->pid == pid) {
            if (signal == SIGSTOP) {
                current->status = 0; // Mark as stopped
            } else if (signal == SIGCONT) {
                current->status = 1; // Mark as running
            }
            if (kill(pid, signal) == -1) {
                perror("kill");
            }
            return;
        }
        current = current->next;
    }
    fprintf(stderr, "No such process with PID %d\n", pid);
}

// Signal handler for SIGCHLD to clean up terminated background processes
void sigchld_handler(int signo) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            printf("Background process with PID %d terminated\n", pid);
            remove_background_process(pid);
        }
    }
}
