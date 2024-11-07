#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>  // Ncurses könyvtár nyíl-billentyűk kezeléséhez és szép felülethez

#define MAX_SOR_HOSSZ 256
#define KULCS_HOSSZ 5
int kulcs[KULCS_HOSSZ] = {5, -14, 31, -9, 3};

// Fájlnevek
#define JELSZO_FAJL "jelszo.bin"
#define TELEFONKONYV_FAJL "telefonkonyv.bin"

// Sor dekódolása
void dekodol_sor(char *sor) {
    for (int i = 0; sor[i] != '\0' && sor[i] != '\n'; i++) {
        sor[i] -= kulcs[i % KULCS_HOSSZ];
    }
}

// Felhasználó hitelesítése jelszó alapján
int hitelesit_felhasznalo(const char *felhasznalo, const char *jelszo) {
    FILE *fajl = fopen(JELSZO_FAJL, "rb");
    if (!fajl) {
        perror("Nem sikerült megnyitni a jelszó fájlt");
        return 0;
    }

    char kodolt_sor[MAX_SOR_HOSSZ];
    char dekodolt_sor[MAX_SOR_HOSSZ];
    int hitelesitett = 0;

    while (fgets(kodolt_sor, MAX_SOR_HOSSZ, fajl)) {
        strcpy(dekodolt_sor, kodolt_sor);
        dekodol_sor(dekodolt_sor);

        char *mentett_felhasznalo = strtok(dekodolt_sor, "*");
        char *mentett_jelszo = strtok(NULL, "\n");

        if (mentett_felhasznalo && mentett_jelszo) {
            if (strcmp(mentett_felhasznalo, felhasznalo) == 0 && strcmp(mentett_jelszo, jelszo) == 0) {
                hitelesitett = 1;
                break;
            }
        }
    }

    fclose(fajl);
    return hitelesitett;
}

// Telefonkönyv böngészése nyilakkal
void bongesz_telefonkonyv() {
    FILE *fajl = fopen(TELEFONKONYV_FAJL, "rb");
    if (!fajl) {
        perror("Nem sikerült megnyitni a telefonkönyvet");
        return;
    }

    char kodolt_sor[MAX_SOR_HOSSZ];
    char dekodolt_sor[MAX_SOR_HOSSZ];
    char telefonkonyv[100][MAX_SOR_HOSSZ];  // Legfeljebb 100 rekord tárolása
    int sorok_szama = 0;

    while (fgets(kodolt_sor, MAX_SOR_HOSSZ, fajl) && sorok_szama < 100) {
        strcpy(dekodolt_sor, kodolt_sor);
        dekodol_sor(dekodolt_sor);
        strcpy(telefonkonyv[sorok_szama++], dekodolt_sor);
    }

    fclose(fajl);

    int pozicio = 0;
    int billentyu;

    initscr();            // Ncurses inicializálása
    cbreak();             // Line-buffer kikapcsolása
    noecho();             // Billentyűk nem jelennek meg
    keypad(stdscr, TRUE); // Nyilak engedélyezése

    do {
        clear();
        mvprintw(0, 0, "Telefonkönyv böngészése (Nyilak a navigáláshoz, q: kilépés)");
        for (int i = 0; i < sorok_szama; i++) {
            if (i == pozicio)
                attron(A_REVERSE);  // Kiemelés a kiválasztott sorra

            mvprintw(i + 2, 0, "%s", telefonkonyv[i]);

            if (i == pozicio)
                attroff(A_REVERSE); // Kiemelés kikapcsolása
        }
        billentyu = getch();
        switch (billentyu) {
            case KEY_UP:
                if (pozicio > 0) pozicio--;
                break;
            case KEY_DOWN:
                if (pozicio < sorok_szama - 1) pozicio++;
                break;
        }
    } while (billentyu != 'q');

    endwin();  // Ncurses bezárása
}

// Főprogram
int main() {
    char felhasznalo[50], jelszo[50];

    printf("Felhasználónév: ");
    scanf("%49s", felhasznalo);
    printf("Jelszó: ");
    scanf("%49s", jelszo);

    if (hitelesit_felhasznalo(felhasznalo, jelszo)) {
        printf("Sikeres bejelentkezés!\n");

        int valasztas;
        do {
            printf("\nMenü:\n");
            printf("1. Telefonkönyv böngészése\n");
            printf("2. Kilépés\n");
            printf("Válassz egy opciót: ");
            scanf("%d", &valasztas);

            switch (valasztas) {
                case 1:
                    bongesz_telefonkonyv();
                    break;
                case 2:
                    printf("Kilépés...\n");
                    break;
                default:
                    printf("Érvénytelen választás, próbáld újra.\n");
            }
        } while (valasztas != 2);
    } else {
        printf("Helytelen felhasználónév vagy jelszó.\n");
    }

    return 0;
}