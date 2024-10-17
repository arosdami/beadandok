#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void listFiles(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("Unable to open directory");
        return;
    }

    printf("Contents of directory: %s\n", path);
    while ((entry = readdir(dp))) {
        printf("%s\n", entry->d_name);
    }

    closedir(dp);
}

void changeDirectory(char *path) {
    if (chdir(path) != 0) {
        perror("chdir() failed");
    }
}

void createFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Unable to create file");
        return;
    }
    printf("File created: %s\n", filename);
    fclose(fp);
}

void createDirectory(const char *dirname) {
    if (mkdir(dirname, 0755) != 0) {
        perror("Unable to create directory");
    } else {
        printf("Directory created: %s\n", dirname);
    }
}

void deleteFile(const char *filename) {
    if (remove(filename) != 0) {
        perror("Unable to delete file");
    } else {
        printf("File deleted: %s\n", filename);
    }
}

void renameFile(const char *oldName, const char *newName) {
    if (rename(oldName, newName) != 0) {
        perror("Unable to rename file");
    } else {
        printf("Renamed: %s -> %s\n", oldName, newName);
    }
}

int main() {
    char command[256], param1[256], param2[256];
    char currentDir[256];
    getcwd(currentDir, sizeof(currentDir));

    while (1) {
        printf("\nFile Manager\n");
        printf("Current directory: %s\n", currentDir);
        printf("Options:\n");
        printf("1. List files\n");
        printf("2. Change directory\n");
        printf("3. Create file\n");
        printf("4. Create directory\n");
        printf("5. Delete file\n");
        printf("6. Rename file\n");
        printf("7. Exit\n");

        printf("Enter a command: ");
        fgets(command, 256, stdin);

        if (strncmp(command, "1", 1) == 0) {
            listFiles(currentDir);
        } else if (strncmp(command, "2", 1) == 0) {
            printf("Enter directory to navigate to: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            changeDirectory(param1);
            getcwd(currentDir, sizeof(currentDir));
        } else if (strncmp(command, "3", 1) == 0) {
            printf("Enter filename to create: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            createFile(param1);
        } else if (strncmp(command, "4", 1) == 0) {
            printf("Enter directory name to create: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            createDirectory(param1);
        } else if (strncmp(command, "5", 1) == 0) {
            printf("Enter filename to delete: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            deleteFile(param1);
        } else if (strncmp(command, "6", 1) == 0) {
            printf("Enter old filename: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            printf("Enter new filename: ");
            fgets(param2, 256, stdin);
            param2[strcspn(param2, "\n")] = '\0';
            renameFile(param1, param2);
        } else if (strncmp(command, "7", 1) == 0) {
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
