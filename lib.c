#include "main.h"

int change_dir(char *path) {
    if (chdir(path) == -1) {
        return -1;
    }
    return 0;
}

void clear_shell() {
    system("clear");
}

void makedir(char *dirname) {
    struct stat st = {0};
    char path[1024];

    snprintf(path, sizeof(path), "./%s", dirname);

    if (stat(path, &st) == -1) {
        if (mkdir(path, 0700) == -1) {
            perror("Failed to create directory");
            exit(EXIT_FAILURE);
        } else {
            printf("Directory created: %s\n", path);
        }
    } else {
        fprintf(stderr, "Directory already exists: %s\n", path);
    }
}

void makefile(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to create file");
        exit(EXIT_FAILURE);
    }

    if (fclose(file) != 0) {
        perror("Failed to close file");
        exit(EXIT_FAILURE);
    }
}

void rename_file(char *old_filename, char *new_filename) {
    if (rename(old_filename, new_filename) == -1) {
        perror("Failed to rename file");
        exit(EXIT_FAILURE);
    }
}

void remove_file(char* filename){
    if (!remove(filename))
        printf("Remove successfully %s\n",filename);
    else{
        perror("Can't remove file\n");
        exit(EXIT_FAILURE);
    }
}

void list_file_contents(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
}

void list_dir(char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        if (strcmp(path, ".") == 0) {
            snprintf(full_path, sizeof(full_path), "%s", entry->d_name);
        } else {
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        }

        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            printf("[DIR]  %s\n", full_path);
        } else if (S_ISREG(entry_stat.st_mode)) {
            printf("[FILE] %s %lldB\n", full_path, (long long) entry_stat.st_size);
        } else {
            printf("[OTHER] %s\n", full_path);
        }
    }

    closedir(dir);
}

void print_path(){
    char *path = getenv("PATH");
    if (path != NULL) {
        printf("PATH: %s\n", path);
    } else {
        printf("The PATH environment variable is not set.\n");
    }

    return;
}

void addpath(char* new_path){  
    char *current_path = getenv("PATH");

    // Calculate the size of the new PATH variable
    size_t new_path_len = strlen(current_path) + strlen(new_path) + 2; // +2 for ':' and null terminator
    char *new_path_env = (char *)malloc(new_path_len);

    // Construct the new PATH value
    snprintf(new_path_env, new_path_len, "%s:%s", current_path, new_path);

    // Set the new PATH value


    // Free the allocated memory
    free(new_path_env);

    return ;
}

int background(char **argv, int argc, process *trace) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process
        setpgid(0, 0); // Set the process group ID to itself to separate it from the shell's process group
        execmd(argv,argc,trace);
        
        exit(0);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printf("Background PID: %d\n", pid); // Debugging statement
        add_background_process(pid, argv[0]);
        return pid;
    }
}

