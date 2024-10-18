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
        perror("Nem sikerült megnyitni a könyvtárat");
        return;
    }

    printf("A könyvtár tartalma: %s\n", path);
    while ((entry = readdir(dp))) {
        printf("%s\n", entry->d_name);
    }

    closedir(dp);

    // Szünetet illesztünk be a felhasználói bemenetre várva
    printf("Nyomjon meg egy billentyűt a folytatáshoz...\n");
    getchar(); // Ez vár a felhasználói bemenetre, hogy a lista látható maradjon
}

void changeDirectory(char *path) {
    if (chdir(path) != 0) {
        perror("Nem sikerült megváltoztatni a könyvtárat");
    }
}

void createFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Nem sikerült létrehozni a fájlt");
        return;
    }
    printf("Fájl létrehozva: %s\n", filename);
    fclose(fp);
}

void createDirectory(const char *dirname) {
    if (mkdir(dirname, 0755) != 0) {
        perror("Nem sikerült létrehozni a könyvtárat");
    } else {
        printf("Könyvtár létrehozva: %s\n", dirname);
    }
}

void deleteFile(const char *filename) {
    if (remove(filename) != 0) {
        perror("Nem sikerült törölni a fájlt");
    } else {
        printf("Fájl törölve: %s\n", filename);
    }
}

void renameFile(const char *oldName, const char *newName) {
    if (rename(oldName, newName) != 0) {
        perror("Nem sikerült átnevezni a fájlt");
    } else {
        printf("Átnevezve: %s -> %s\n", oldName, newName);
    }
}

int main() {
    char command[256], param1[256], param2[256];
    char currentDir[256];
    getcwd(currentDir, sizeof(currentDir));

    while (1) {
        printf("\nFájlkezelő\n");
        printf("Jelenlegi könyvtár: %s\n", currentDir);
        printf("Opciók:\n");
        printf("1. Fájlok listázása\n");
        printf("2. Könyvtárváltás\n");
        printf("3. Fájl létrehozása\n");
        printf("4. Könyvtár létrehozása\n");
        printf("5. Fájl törlése\n");
        printf("6. Fájl átnevezése\n");
        printf("7. Kilépés\n");

        printf("Adja meg a parancsot: ");
        fgets(command, 256, stdin);

        if (strncmp(command, "1", 1) == 0) {
            listFiles(currentDir);
        } else if (strncmp(command, "2", 1) == 0) {
            printf("Adja meg a könyvtárat: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            changeDirectory(param1);
            getcwd(currentDir, sizeof(currentDir));
        } else if (strncmp(command, "3", 1) == 0) {
            printf("Adja meg a fájl nevét: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            createFile(param1);
        } else if (strncmp(command, "4", 1) == 0) {
            printf("Adja meg a könyvtár nevét: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            createDirectory(param1);
        } else if (strncmp(command, "5", 1) == 0) {
            printf("Adja meg a törlendő fájl nevét: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            deleteFile(param1);
        } else if (strncmp(command, "6", 1) == 0) {
            printf("Adja meg a régi fájl nevét: ");
            fgets(param1, 256, stdin);
            param1[strcspn(param1, "\n")] = '\0';
            printf("Adja meg az új fájl nevét: ");
            fgets(param2, 256, stdin);
            param2[strcspn(param2, "\n")] = '\0';
            renameFile(param1, param2);
        } else if (strncmp(command, "7", 1) == 0) {
            break;
        } else {
            printf("Érvénytelen parancs\n");
        }
    }

    return 0;
}
