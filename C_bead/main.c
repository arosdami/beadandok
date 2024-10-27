#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define KEY_LENGTH 5
int key[KEY_LENGTH] = {5, -14, 31, -9, 3};  // Titkosítási kulcs

// Fájl elérési útvonalak
#define JELSZO_FAJL "/home/digit/Public/C_bead/jelszo.bin"
#define TELEFONKONYV_FAJL "/home/digit/Public/C_bead/telefonkonyv.bin"

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

// Keresés a telefonkönyvben
void search_in_phonebook(const char *query) {
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

        if (strstr(decoded_line, query)) {
            printf("Találat: %s\n", decoded_line);
            found = 1;
        }
    }

    if (!found) {
        printf("Nincs találat a keresésre.\n");
    }

    fclose(file);
}

// Főprogram
int main() {
    char username[50], password[50];

    printf("Felhasználónév: ");
    scanf("%49s", username);
    printf("Jelszó: ");
    scanf("%49s", password);

    if (authenticate_user(username, password)) {
        printf("Sikeres bejelentkezés!\n");

        int choice;
        do {
            printf("\nMenü:\n");
            printf("1. Telefonkönyv böngészése\n");
            printf("2. Keresés név/telefonszám alapján\n");
            printf("3. Kilépés\n");
            printf("Válassz egy opciót: ");
            scanf("%d", &choice);

            char query[50];
            switch (choice) {
                case 1:
                    browse_phonebook();
                    break;
                case 2:
                    printf("Keresett név vagy telefonszám: ");
                    scanf("%49s", query);
                    search_in_phonebook(query);
                    break;
                case 3:
                    printf("Kilépés...\n");
                    break;
                default:
                    printf("Érvénytelen választás, próbáld újra.\n");
            }
        } while (choice != 3);
    } else {
        printf("Helytelen felhasználónév vagy jelszó.\n");
    }

    return 0;
}
