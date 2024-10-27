#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define KEY_LENGTH 5
int key[KEY_LENGTH] = {5, -14, 31, -9, 3};  // Dekódoló kulcs

// Dekódoló függvény
void decode_line(char *line) {
    for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
        line[i] -= key[i % KEY_LENGTH];
    }
}

// Felhasználó hitelesítése jelszó alapján
int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(JELSZO_FAJL, "rb");
    if (!file) {
        perror("Nem sikerült megnyitni a jelszó fájlt");
        return 0;
    }

    char encoded_line[MAX_LINE_LENGTH];
    char decoded_line[MAX_LINE_LENGTH];
    int auth_success = 0;

    while (fgets(encoded_line, MAX_LINE_LENGTH, file)) {
        strcpy(decoded_line, encoded_line);
        decode_line(decoded_line);

        // Felhasználónév és jelszó különválasztása
        char *stored_username = strtok(decoded_line, "*");
        char *stored_password = strtok(NULL, "\n");

        if (stored_username && stored_password) {
            if (strcmp(stored_username, username) == 0 && strcmp(stored_password, password) == 0) {
                auth_success = 1;
                break;
            }
        }
    }

    fclose(file);
    return auth_success;
}

// Telefonkönyv keresési funkció
void search_in_phonebook(const char *query, int search_by_name) {
    FILE *file = fopen(TELEFONKONYV_FAJL, "rb");
    if (!file) {
        perror("Nem sikerült megnyitni a telefonkönyvet");
        return;
    }

    char encoded_line[MAX_LINE_LENGTH];
    char decoded_line[MAX_LINE_LENGTH];
    int found = 0;

    while (fgets(encoded_line, MAX_LINE_LENGTH, file)) {
        strcpy(decoded_line, encoded_line);
        decode_line(decoded_line);

        // Keresés név vagy telefonszám alapján
        if ((search_by_name && strstr(decoded_line, query)) || (!search_by_name && strstr(decoded_line, query))) {
            printf("Találat: %s\n", decoded_line);
            found = 1;
        }
    }

    if (!found) {
        printf("Nincs találat a keresésre.\n");
    }

    fclose(file);
}

// Telefonkönyv böngészése
void browse_phonebook() {
    FILE *file = fopen(TELEFONKONYV_FAJL, "rb");
    if (!file) {
        perror("Nem sikerült megnyitni a telefonkönyvet");
        return;
    }

    char encoded_line[MAX_LINE_LENGTH];
    char decoded_line[MAX_LINE_LENGTH];

    printf("Telefonkönyv tartalma:\n");
    while (fgets(encoded_line, MAX_LINE_LENGTH, file)) {
        strcpy(decoded_line, encoded_line);
        decode_line(decoded_line);
        printf("%s\n", decoded_line);
    }

    fclose(file);
}

// Menü megjelenítése
void display_menu() {
    printf("\nMenü:\n");
    printf("1. Keresés névre\n");
    printf("2. Keresés telefonszámra\n");
    printf("3. Telefonkönyv böngészése\n");
    printf("4. Kilépés\n");
}

// Főprogram
int main() {
    char username[50], password[50];

    // Felhasználói név és jelszó bekérése
    printf("Felhasználónév: ");
    scanf("%49s", username);
    printf("Jelszó: ");
    scanf("%49s", password);

    // Hitelesítés
    if (authenticate_user(username, password)) {
        printf("Sikeres bejelentkezés!\n");

        int choice;
        do {
            display_menu();
            printf("Válasszon egy opciót: ");
            scanf("%d", &choice);

            char query[50];
            switch (choice) {
                case 1:
                    printf("Keresett név: ");
                    scanf("%49s", query);
                    search_in_phonebook(query, 1);
                    break;
                case 2:
                    printf("Keresett telefonszám: ");
                    scanf("%49s", query);
                    search_in_phonebook(query, 0);
                    break;
                case 3:
                    browse_phonebook();
                    break;
                case 4:
                    printf("Kilépés...\n");
                    break;
                default:
                    printf("Érvénytelen választás, próbálja újra.\n");
            }
        } while (choice != 4);
    } else {
        printf("Helytelen felhasználónév vagy jelszó.\n");
    }

    return 0;
}
