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
void file_operations();
void directory_operations();
void keylogger_operations();
void run_in_background();

int main() {
    int choice;

    while (1) {
        // Main menu system
        printf("\n--- Supercommand Menu ---\n");
        printf("1. File Operations\n");
        printf("2. Directory Operations\n");
        printf("3. Keylogger Operations\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                file_operations();
                break;
            case 2:
                directory_operations();
                break;
            case 3:
                keylogger_operations();
                break;
            case 4:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

// File operations submenu
void file_operations() {
    int choice;
    char filename[200];
    char content[1024];
    int fd;
    mode_t new_permissions;

    while (1) {
        printf("\n--- File Operations ---\n");
        printf("1. Create/Open a new file\n");
        printf("2. Change file permissions\n");
        printf("3. Read from a file\n");
        printf("4. Write to a file\n");
        printf("5. Remove a file\n");
        printf("6. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Create/Open a file
                printf("Enter the filename (with optional path): ");
                scanf("%s", filename);
                fd = open(filename, O_CREAT | O_WRONLY, 0644);
                if (fd < 0) {
                    perror("Error creating/opening file");
                } else {
                    printf("File '%s' created/opened successfully.\n", filename);
                    close(fd);
                }
                break;

            case 2: // Change file permissions
                printf("Enter the filename (with optional path): ");
                scanf("%s", filename);
                printf("Enter the new permissions (in octal, e.g., 755): ");
                scanf("%o", &new_permissions);
                if (chmod(filename, new_permissions) == 0) {
                    printf("Permissions for '%s' changed successfully.\n", filename);
                } else {
                    perror("Error changing file permissions");
                }
                break;

            case 3: // Read from a file
                printf("Enter the filename to read: ");
                scanf("%s", filename);
                fd = open(filename, O_RDONLY);
                if (fd < 0) {
                    perror("Error opening file");
                } else {
                    printf("Contents of '%s':\n", filename);
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
                printf("Enter the filename to write to: ");
                scanf("%s", filename);
                fd = open(filename, O_WRONLY | O_APPEND);
                if (fd < 0) {
                    perror("Error opening file");
                } else {
                    printf("Enter the text to write to the file: ");
                    getchar();  // Consume newline left by scanf
                    fgets(content, sizeof(content), stdin);
                    if (write(fd, content, strlen(content)) < 0) {
                        perror("Error writing to file");
                    } else {
                        printf("Text written to '%s' successfully.\n", filename);
                    }
                    close(fd);
                }
                break;

            case 5: // Remove a file
                printf("Enter the filename (with optional path) to delete: ");
                scanf("%s", filename);
                if (remove(filename) == 0) {
                    printf("File '%s' deleted successfully.\n", filename);
                } else {
                    perror("Error deleting file");
                }
                break;

            case 6: // Back to main menu
                return;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Directory operations submenu
void directory_operations() {
    int choice;
    char path[200];
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];

    while (1) {
        printf("\n--- Directory Operations ---\n");
        printf("1. Create a directory\n");
        printf("2. Delete a directory\n");
        printf("3. Print current working directory\n");
        printf("4. List current directory and files\n");
        printf("5. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Create a directory
                printf("Enter the directory name (with optional path): ");
                scanf("%s", path);
                if (mkdir(path, 0755) == 0) {
                    printf("Directory '%s' created successfully.\n", path);
                } else {
                    perror("Error creating directory");
                }
                break;

            case 2: // Delete a directory
                printf("Enter the directory name (with optional path) to delete: ");
                scanf("%s", path);
                if (rmdir(path) == 0) {
                    printf("Directory '%s' deleted successfully.\n", path);
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
                printf("Enter the directory to list ('.' for current): ");
                scanf("%s", path);
                dir = opendir(path);
                if (dir == NULL) {
                    perror("Error opening directory");
                } else {
                    printf("Contents of '%s':\n", path);
                    while ((entry = readdir(dir)) != NULL) {
                        printf("%s\n", entry->d_name);
                    }
                    closedir(dir);
                }
                break;

            case 5: // Back to main menu
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}

// Keylogger operations
void keylogger_operations() {
    printf("\n--- Keylogger Operations ---\n");
    printf("Keylogger will now run in the background.\n");
    run_in_background();
}

// Function to run the keylogger in the background
void run_in_background() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Failed to fork process");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        printf("Keylogger process started in the background. PID: %d\n", pid);
        return;
    }

    // Child process
    // Run keylogger functionality
    int fd = open("keylog.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening keylog file");
        exit(EXIT_FAILURE);
    }

    // Add a timestamp at the beginning of the session
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    dprintf(fd, "\n--- Keylogger Session Started: %s---\n", timestamp);

    char c;
    while (1) {
        // Capture keystrokes using getchar() (basic implementation)
        c = getchar();
        if (c != EOF) {
            write(fd, &c, 1);
        }
    }

    close(fd);
    exit(0);
}
