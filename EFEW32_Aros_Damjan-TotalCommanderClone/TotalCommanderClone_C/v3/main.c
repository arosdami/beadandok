#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>

#define COLOR_RESET "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_CYAN "\x1B[36m"
#define COLOR_BOLD "\x1B[1m"

void listFiles(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("Unable to open directory");
        return;
    }

    printf(COLOR_CYAN "Directory contents: %s\n" COLOR_RESET, path);
    while ((entry = readdir(dp))) {
        printf(COLOR_GREEN "%s\n" COLOR_RESET, entry->d_name);
    }

    closedir(dp);
}

void changeDirectory(char *path) {
    if (chdir(path) != 0) {
        perror("chdir() failed");
    } else {
        printf(COLOR_YELLOW "Changed directory to: %s\n" COLOR_RESET, path);
    }
}

void createFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Unable to create file");
        return;
    }
    printf(COLOR_GREEN "File created: %s\n" COLOR_RESET, filename);
    fclose(fp);
}

void createDirectory(const char *dirname) {
    if (mkdir(dirname, 0755) != 0) {
        perror("Unable to create directory");
    } else {
        printf(COLOR_GREEN "Directory created: %s\n" COLOR_RESET, dirname);
    }
}

void deleteFile(const char *filename) {
    if (remove(filename) != 0) {
        perror("Unable to delete file");
    } else {
        printf(COLOR_RED "File deleted: %s\n" COLOR_RESET, filename);
    }
}

void renameFile(const char *oldName, const char *newName) {
    if (rename(oldName, newName) != 0) {
        perror("Unable to rename file");
    } else {
        printf(COLOR_YELLOW "Renamed: %s -> %s\n" COLOR_RESET, oldName, newName);
    }
}

// Beállítja a terminált karakterenkénti olvasásra (nyílbillentyűk kezeléséhez)
void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Canonikus mód és echo kikapcsolása
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Visszaállítja a terminált normál módba
void disableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Canonikus mód és echo visszaállítása
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Menü megjelenítése kerettel és kiválasztás nyilakkal
void displayMenu(int selected) {
    char *menuItems[] = {
        "List files",
        "Change directory",
        "Create file",
        "Create directory",
        "Delete file",
        "Rename file",
        "Exit"
    };
    int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

    printf(COLOR_BOLD "\nFile Manager\n" COLOR_RESET);
    printf(COLOR_CYAN "+---------------------------+\n" COLOR_RESET);

    for (int i = 0; i < numItems; i++) {
        if (i == selected) {
            printf(COLOR_YELLOW "-> %-25s\n" COLOR_RESET, menuItems[i]);
        } else {
            printf("   %-25s\n", menuItems[i]);
        }
    }

    printf(COLOR_CYAN "+---------------------------+\n" COLOR_RESET);
}

int main() {
    char param1[256], param2[256];
    char currentDir[256];
    int selected = 0;
    getcwd(currentDir, sizeof(currentDir));

    enableRawMode(); // Nyílbillentyű kezeléséhez szükséges
    while (1) {
        displayMenu(selected);

        char c = getchar();
        if (c == '\033') { // Nyílbillentyűk kódja: ESC karakter (0x1B)
            getchar(); // Kilépési jel
            switch (getchar()) {
                case 'A': // Fel nyíl
                    selected = (selected - 1 + 7) % 7;
                    break;
                case 'B': // Le nyíl
                    selected = (selected + 1) % 7;
                    break;
            }
        } else if (c == '\n') { // Enter lenyomása
            if (selected == 0) {
                listFiles(currentDir);
            } else if (selected == 1) {
                printf("Enter directory to navigate to: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                changeDirectory(param1);
                getcwd(currentDir, sizeof(currentDir));
            } else if (selected == 2) {
                printf("Enter filename to create: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                createFile(param1);
            } else if (selected == 3) {
                printf("Enter directory name to create: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                createDirectory(param1);
            } else if (selected == 4) {
                printf("Enter filename to delete: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                deleteFile(param1);
            } else if (selected == 5) {
                printf("Enter old filename: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                printf("Enter new filename: ");
                fgets(param2, 256, stdin);
                param2[strcspn(param2, "\n")] = '\0';
                renameFile(param1, param2);
            } else if (selected == 6) {
                break;
            }
        }

        system("clear"); // Képernyő törlése a menü újrarajzolásához
    }

    disableRawMode(); // Nyers mód letiltása

    return 0;
}
