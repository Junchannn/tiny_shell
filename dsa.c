#include "main.h"

// Global process list
process* head = NULL;

int main() {
    char *input = NULL;
    size_t len = 0;
    ssize_t nread;
    char *argv[64];
    int argc;
    char cwd[1024];

    // Set up the SIGCHLD handler
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Print the process list

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("(shell) %s $ ", cwd);
        } else {
            perror("getcwd() error");
            break;
        }

        nread = getline(&input, &len, stdin);
        if (nread == -1) {
            perror("getline");
            break;
        }

        input[nread - 1] = '\0'; // Remove newline character

        argc = 0;
        argv[argc] = strtok(input, " ");
        while (argv[argc] != NULL) {
            argc++;
            argv[argc] = strtok(NULL, " ");
        }

        if (argc == 0) {
            continue;
        }

        if (strcmp(argv[argc - 1], "&") == 0) {
            argv[argc - 1] = NULL;
            argc--;
            background(argv, argc, head);
        } else {
            if (!strcmp(argv[0], "cd")) {
                if (argc == 1) {
                    if (chdir("/home/junchannn") == -1) {
                        perror("cd");
                    }
                } else {
                    if (change_dir(argv[1]) == -1) {
                        perror("cd");
                    }
                }
            } else {
                pid_t pid = fork();

                if (pid < 0) {
                    perror("Fork failed");
                } else if (pid == 0) {
                    execmd(argv, argc, head);
                    exit(EXIT_FAILURE);
                } else {
                    waitpid(pid, NULL, 0);
                }
            }
        }
    }
 //test
    free(input);
    

    return 0;
}
