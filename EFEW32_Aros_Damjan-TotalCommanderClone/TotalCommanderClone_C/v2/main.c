#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>

// A terminál mód visszaállítása a speciális bevitel (pl. nyilak) kezelése után
void visszaallit_terminal_mod(struct termios *eredeti_attrib) {
    tcsetattr(STDIN_FILENO, TCSANOW, eredeti_attrib);
}

// A terminál mód beállítása speciális bevitelek (pl. nyilak) kezelésére
void beallit_conio_terminal_mod() {
    struct termios uj_attrib;
    tcgetattr(STDIN_FILENO, &uj_attrib);
    uj_attrib.c_lflag &= ~(ICANON | ECHO); // Pufferezés és visszhang kikapcsolása
    uj_attrib.c_cc[VMIN] = 1;              // Minimum 1 karakteres bevitel
    uj_attrib.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &uj_attrib);
}

// Nyilak olvasása (escape szekvencia)
int olvas_nyil() {
    int gomb = getchar();
    if (gomb == '\033') { // Escape szekvencia
        getchar(); // További escape karakterek átugrása
        switch(getchar()) { // Ellenőrizzük, hogy melyik nyílgombot nyomták meg
            case 'A': return 1; // Fel nyíl
            case 'B': return 2; // Le nyíl
        }
    }
    return 0; // Nem nyíl gombot nyomtak meg
}

// Fájlok listázása
void listazFajlokat(const char *utvonal) {
    struct dirent *bejegyzes;
    DIR *dp = opendir(utvonal);

    if (dp == NULL) {
        perror("Nem sikerült megnyitni a könyvtárat");
        return;
    }

    printf("Könyvtár tartalma: %s\n", utvonal);
    while ((bejegyzes = readdir(dp))) {
        printf("%s\n", bejegyzes->d_name);
    }

    closedir(dp);
}

// Könyvtár váltása
void valtsKonyvtar(char *utvonal) {
    if (chdir(utvonal) != 0) {
        perror("chdir() hiba");
    }
}

// Fájl létrehozása
void hozLetFajlt(const char *fajlnev) {
    FILE *fp = fopen(fajlnev, "w");
    if (fp == NULL) {
        perror("Nem sikerült létrehozni a fájlt");
        return;
    }
    printf("Fájl létrehozva: %s\n", fajlnev);
    fclose(fp);
}

// Könyvtár létrehozása
void hozLetKonyvtarat(const char *konyvtarnev) {
    if (mkdir(konyvtarnev, 0755) != 0) {
        perror("Nem sikerült létrehozni a könyvtárat");
    } else {
        printf("Könyvtár létrehozva: %s\n", konyvtarnev);
    }
}

// Fájl törlése
void torolFajlt(const char *fajlnev) {
    if (remove(fajlnev) != 0) {
        perror("Nem sikerült törölni a fájlt");
    } else {
        printf("Fájl törölve: %s\n", fajlnev);
    }
}

// Fájl átnevezése
void nevezAtFajlt(const char *regiNev, const char *ujNev) {
    if (rename(regiNev, ujNev) != 0) {
        perror("Nem sikerült átnevezni a fájlt");
    } else {
        printf("Átnevezve: %s -> %s\n", regiNev, ujNev);
    }
}

// Fájlkezelő menü
int main() {
    char parancs[256], param1[256], param2[256];
    char aktualisKonyvtar[256];
    int valasztas = 1;
    getcwd(aktualisKonyvtar, sizeof(aktualisKonyvtar));

    struct termios eredeti_attrib;
    tcgetattr(STDIN_FILENO, &eredeti_attrib);
    beallit_conio_terminal_mod();

    while (1) {
        system("clear"); // Töröljük a képernyőt a keret újrarajzolásához
        printf("*********** Fájlkezelő ***********\n");
        printf("Aktuális könyvtár: %s\n", aktualisKonyvtar);
        printf("*******************************\n");

        // Menü megjelenítése és a választás jelzése
        printf("%s 1. Fájlok listázása\n", valasztas == 1 ? "->" : "  ");
        printf("%s 2. Könyvtár váltása\n", valasztas == 2 ? "->" : "  ");
        printf("%s 3. Fájl létrehozása\n", valasztas == 3 ? "->" : "  ");
        printf("%s 4. Könyvtár létrehozása\n", valasztas == 4 ? "->" : "  ");
        printf("%s 5. Fájl törlése\n", valasztas == 5 ? "->" : "  ");
        printf("%s 6. Fájl átnevezése\n", valasztas == 6 ? "->" : "  ");
        printf("%s 7. Kilépés\n", valasztas == 7 ? "->" : "  ");
        printf("*******************************\n");

        // Nyilak kezelése
        int gomb = olvas_nyil();
        if (gomb == 1) { // Fel nyíl
            if (valasztas > 1) valasztas--;
        } else if (gomb == 2) { // Le nyíl
            if (valasztas < 7) valasztas++;
        } else if (gomb == 0) { // Enter gomb
            if (valasztas == 1) {
                listazFajlokat(aktualisKonyvtar);
            } else if (valasztas == 2) {
                printf("Add meg a könyvtár nevét: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                valtsKonyvtar(param1);
                getcwd(aktualisKonyvtar, sizeof(aktualisKonyvtar));
            } else if (valasztas == 3) {
                printf("Add meg a fájl nevét: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                hozLetFajlt(param1);
            } else if (valasztas == 4) {
                printf("Add meg a könyvtár nevét: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\lo';
                hozLetKonyvtarat(param1);
            } else if (valasztas == 5) {
                printf("Add meg a törlendő fájl nevét: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                torolFajlt(param1);
            } else if (valasztas == 6) {
                printf("Add meg a régi fájl nevét: ");
                fgets(param1, 256, stdin);
                param1[strcspn(param1, "\n")] = '\0';
                printf("Add meg az új fájl nevét: ");
                fgets(param2, 256, stdin);
                param2[strcspn(param2, "\n")] = '\0';
                nevezAtFajlt(param1, param2);
            } else if (valasztas == 7) {
                break;
            }
            printf("\nNyomj meg egy gombot a folytatáshoz...");
            getchar(); // Várakozás egy gombnyomásra
        }
    }

    // A terminál mód visszaállítása a program végén
    visszaallit_terminal_mod(&eredeti_attrib);
    return 0;
}
