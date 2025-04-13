#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXIM_CARTI 500
#define FISIER "biblioteca.txt"

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"

// structura carte
typedef struct {
    char titlu[100];
    char autor[100];
    char tip[50];       // Ex: roman, nuvela, poezie etc.
    char locatie[50];   // Ex: raftul X, sala de lectura etc.
    int disponibil;     // 1 = disponibil, 0 = împrumutat
    int zile_ramase;    // numărul de zile ramase daca este imprumutata (max 10)
    char dataAchizitie[11];
} Carte;

Carte carti[MAXIM_CARTI];
int nrCarti = 0;

//functie de curatare a ecranului
void clearScreen() {
    system("clear"); // sau cls pe windows
}

int validareData(const char* data) {
    // trebuie sa fie lungimea exact 10: dd/mm/yyyy
    // indicii: 0 1 2 3 4 5 6 7 8 9
    if (strlen(data) != 10) {
        return 0;
    }
    // verificam ca pozitiile 2 si 5 sa fie '/'
    if (data[2] != '/' || data[5] != '/') {
        return 0;
    }
    // si dupa verificam ca restul pozitiilor sa fie cifre, de asta si folosim ctype.h
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue; // acestea trebuie să fie '/'
        if (!isdigit((unsigned char)data[i])) {
            return 0;
        }
    }
    // daca am ajuns aici, formatul de baza este in regula
    return 1;
}

void incarcaDateDinFisier() {
    FILE *f = fopen(FISIER, "r");
    if (!f) {
        // Dacă fișierul nu există, nu facem nimic, se va crea ulterior
        return;
    }
    
    nrCarti = 0;
    while (!feof(f)) {
        char linie[300];
        if (fgets(linie, sizeof(linie), f) != NULL) {
            // Eliminăm eventualul '\n' de la finalul liniei
            linie[strcspn(linie, "\n")] = '\0';
            
            // Extragem câmpurile pe baza delimitatorului "|"
            char *p = strtok(linie, "|");
            if (p == NULL) continue;
            strcpy(carti[nrCarti].titlu, p);
            
            p = strtok(NULL, "|");
            if (p == NULL) continue;
            strcpy(carti[nrCarti].autor, p);
            
            p = strtok(NULL, "|");
            if (p == NULL) continue;
            strcpy(carti[nrCarti].tip, p);
            
            p = strtok(NULL, "|");
            if (p == NULL) continue;
            strcpy(carti[nrCarti].locatie, p);
            
            p = strtok(NULL, "|");
            if (p == NULL) continue;
            carti[nrCarti].disponibil = atoi(p);
            
            p = strtok(NULL, "|");
            if (p == NULL) continue;
            carti[nrCarti].zile_ramase = atoi(p);

            p = strtok(NULL, "|");
            if (p == NULL) continue;
            strcpy(carti[nrCarti].dataAchizitie, p);
            
            nrCarti++;
            if (nrCarti >= MAXIM_CARTI) {
                break; // am atins limita maximă
            }
        }
    }
    fclose(f);
}

void salveazaDateInFisier() {
    FILE *f = fopen(FISIER, "w");
    if (!f) {
        printf("Eroare la deschiderea fișierului pentru scriere!\n");
        return;
    }
    
    for (int i = 0; i < nrCarti; i++) {
        fprintf(f, "%s|%s|%s|%s|%d|%d|%s\n",
                carti[i].titlu,
                carti[i].autor,
                carti[i].tip,
                carti[i].locatie,
                carti[i].disponibil,
                carti[i].zile_ramase,
                carti[i].dataAchizitie);
    }
    
    fclose(f);
}

void afiseazaToateCartile() {
    if (nrCarti == 0) {
        printf("Nu există cărți în bibliotecă.\n");
        return;
    }
    for (int i = 0; i < nrCarti; i++) {
        printf("\n--- Cartea #%d ---\n", i + 1);
        printf("Titlu: " COLOR_CYAN "%s\n" COLOR_RESET, carti[i].titlu);
        printf("Autor: " COLOR_MAGENTA "%s\n" COLOR_RESET, carti[i].autor);
        printf("Tip: " COLOR_YELLOW "%s\n" COLOR_RESET, carti[i].tip);
        printf("Locație: " COLOR_BLUE "%s\n" COLOR_RESET, carti[i].locatie);
        if (carti[i].disponibil == 1) {
            printf("Stare: " COLOR_GREEN "Disponibilă\n" COLOR_RESET);
        } else {
            printf("Stare: " COLOR_RED "Împrumutată (%d zile rămase)\n" COLOR_RESET, carti[i].zile_ramase);
        }
        printf("Data achiziției: " COLOR_GREEN "%s\n" COLOR_RESET, carti[i].dataAchizitie);
    }
}

void adaugaCarte() {
    if (nrCarti >= MAXIM_CARTI) {
        printf("Nu se mai pot adăuga cărți, limita atinsă!\n");
        return;
    }

    // citim datele pentru noua carte
    printf("Introduceți titlul: ");
    fgets(carti[nrCarti].titlu, sizeof(carti[nrCarti].titlu), stdin);
    carti[nrCarti].titlu[strcspn(carti[nrCarti].titlu, "\n")] = '\0'; // eliminăm \n
    
    printf("Introduceți autorul: ");
    fgets(carti[nrCarti].autor, sizeof(carti[nrCarti].autor), stdin);
    carti[nrCarti].autor[strcspn(carti[nrCarti].autor, "\n")] = '\0';
    
    printf("Introduceți tipul (ex: roman, poezie etc.): ");
    fgets(carti[nrCarti].tip, sizeof(carti[nrCarti].tip), stdin);
    carti[nrCarti].tip[strcspn(carti[nrCarti].tip, "\n")] = '\0';
    
    printf("Introduceți locația (ex: raftul 3): ");
    fgets(carti[nrCarti].locatie, sizeof(carti[nrCarti].locatie), stdin);
    carti[nrCarti].locatie[strcspn(carti[nrCarti].locatie, "\n")] = '\0';
    
    while (1) {
        printf("Introduceți data achiziției (dd/mm/yyyy): ");
        fgets(carti[nrCarti].dataAchizitie, sizeof(carti[nrCarti].dataAchizitie), stdin);
        carti[nrCarti].dataAchizitie[strcspn(carti[nrCarti].dataAchizitie, "\n")] = '\0';

        if (validareData(carti[nrCarti].dataAchizitie)) {
            break; // format valid, ieșim din while
        } else {
            printf("Format invalid! Vă rog reintroduceți data în formatul dd/mm/yyyy.\n");
        }
    }

    // se marcheaza cartea ca fiind disponibila
    carti[nrCarti].disponibil = 1;
    carti[nrCarti].zile_ramase = 0;
    
    nrCarti++;
    printf("Cartea a fost adăugată cu succes!\n");
}

void cautaDupaTitlu() {
    char titluCautat[100];
    printf("Introduceți titlul (sau parte din titlu) căutat: ");
    fgets(titluCautat, sizeof(titluCautat), stdin);
    titluCautat[strcspn(titluCautat, "\n")] = '\0';
    
    int gasit = 0;
    for (int i = 0; i < nrCarti; i++) {
        if (strstr(carti[i].titlu, titluCautat) != NULL) {
            gasit = 1;
            printf("\n--- Cartea #%d ---\n", i + 1);
            printf("Titlu: " COLOR_CYAN "%s\n" COLOR_RESET, carti[i].titlu);
            printf("Autor: " COLOR_MAGENTA "%s\n" COLOR_RESET, carti[i].autor);
            printf("Tip: " COLOR_YELLOW "%s\n" COLOR_RESET, carti[i].tip);
            printf("Locație: " COLOR_BLUE "%s\n" COLOR_RESET, carti[i].locatie);
            if (carti[i].disponibil == 1) {
                printf("Stare: " COLOR_GREEN "Disponibilă\n" COLOR_RESET);
            } else {
                printf("Stare: " COLOR_RED "Împrumutată (%d zile rămase)\n" COLOR_RESET, carti[i].zile_ramase);
            }
            printf("Data achiziției: " COLOR_GREEN "%s\n" COLOR_RESET, carti[i].dataAchizitie);
        }
    }
    
    if (!gasit) {
        printf("Nu s-au găsit cărți care să conțină în titlu: %s\n", titluCautat);
    }
}

void cautaDupaAutor() {
    char autorCautat[100];
    printf("Introduceți autorul (sau parte din nume) căutat: ");
    fgets(autorCautat, sizeof(autorCautat), stdin);
    autorCautat[strcspn(autorCautat, "\n")] = '\0';
    
    int gasit = 0;
    for (int i = 0; i < nrCarti; i++) {
        if (strstr(carti[i].autor, autorCautat) != NULL) {
            gasit = 1;
            printf("\n--- Cartea #%d ---\n", i + 1);
            printf("Titlu: " COLOR_CYAN "%s\n" COLOR_RESET, carti[i].titlu);
            printf("Autor: " COLOR_MAGENTA "%s\n" COLOR_RESET, carti[i].autor);
            printf("Tip: " COLOR_YELLOW "%s\n" COLOR_RESET, carti[i].tip);
            printf("Locație: " COLOR_BLUE "%s\n" COLOR_RESET, carti[i].locatie);
            if (carti[i].disponibil == 1) {
                printf("Stare: " COLOR_GREEN "Disponibilă\n" COLOR_RESET);
            } else {
                printf("Stare: " COLOR_RED "Împrumutată (%d zile rămase)\n" COLOR_RESET, carti[i].zile_ramase);
            }
            printf("Data achiziției: " COLOR_GREEN "%s\n" COLOR_RESET, carti[i].dataAchizitie);
        }
    }
    
    if (!gasit) {
        printf("Nu s-au găsit cărți scrise de autorul: %s\n", autorCautat);
    }
}

void afiseazaCartiDisponibile() {
    int gasit = 0;
    for (int i = 0; i < nrCarti; i++) {
        if (carti[i].disponibil == 1) {
            gasit = 1;
            printf("\n--- Cartea #%d ---\n", i + 1);
            printf("Titlu: " COLOR_CYAN "%s\n" COLOR_RESET, carti[i].titlu);
            printf("Autor: " COLOR_MAGENTA "%s\n" COLOR_RESET, carti[i].autor);
            printf("Tip: " COLOR_YELLOW "%s\n" COLOR_RESET, carti[i].tip);
            printf("Locație: " COLOR_BLUE "%s\n" COLOR_RESET, carti[i].locatie);
            printf("Stare: " COLOR_GREEN "Disponibilă\n" COLOR_RESET);
            printf("Data achiziției: " COLOR_GREEN "%s\n" COLOR_RESET, carti[i].dataAchizitie);
        }
    }
    if (!gasit) {
        printf("Nu există cărți disponibile în acest moment.\n");
    }
}

void imprumutaCarte() {
    if (nrCarti == 0) {
        printf("Nu există cărți în bibliotecă.\n");
        return;
    }
    // Afișăm mai întâi cărțile disponibile:
    int gasitDisponibile = 0;
    for (int i = 0; i < nrCarti; i++) {
        if (carti[i].disponibil == 1) {
            gasitDisponibile = 1;
        }
    }
    if (!gasitDisponibile) {
        printf("Nu există nicio carte disponibilă pentru împrumut.\n");
        return;
    }
    
    printf("Introduceți indexul cărții (așa cum apare în listă) de împrumutat: ");
    int index;
    scanf("%d", &index);
    getchar(); // consumăm '\n' rămas după scanf

    // Validăm indexul
    if (index < 1 || index > nrCarti) {
        printf("Index invalid!\n");
        return;
    }
    // Intern, vectorul e 0-based, dar utilizatorului i se afișează 1-based
    index--; 
    
    // Verificăm dacă e disponibilă
    if (carti[index].disponibil == 0) {
        printf("Cartea \"%s\" este deja împrumutată.\n", carti[index].titlu);
        return;
    }
    
    // Împrumutăm cartea pentru 10 zile
    carti[index].disponibil = 0;
    carti[index].zile_ramase = 10;
    printf("Cartea \"%s\" a fost împrumutată pentru 10 zile.\n", carti[index].titlu);
}

void returneazaCarte() {
    if (nrCarti == 0) {
        printf("Nu există cărți în bibliotecă.\n");
        return;
    }
    // Afișăm doar cărțile împrumutate
    int gasitImprumutate = 0;
    for (int i = 0; i < nrCarti; i++) {
        if (carti[i].disponibil == 0) {
            gasitImprumutate = 1;
            break;
        }
    }
    if (!gasitImprumutate) {
        printf("Nu există nicio carte împrumutată în acest moment.\n");
        return;
    }
    
    printf("Introduceți indexul cărții (așa cum apare în listă) de returnat: ");
    int index;
    scanf("%d", &index);
    getchar(); // consumăm '\n'
    
    if (index < 1 || index > nrCarti) {
        printf("Index invalid!\n");
        return;
    }
    index--;
    
    // Verificăm dacă e deja disponibilă
    if (carti[index].disponibil == 1) {
        printf("Cartea \"%s\" nu este împrumutată.\n", carti[index].titlu);
        return;
    }
    
    // Returnăm cartea
    carti[index].disponibil = 1;
    carti[index].zile_ramase = 0;
    printf("Cartea \"%s\" a fost returnată.\n", carti[index].titlu);
}

void submeniuCautare() {
    while (1) {
        clearScreen();
        printf("===== Submeniu Căutare =====\n");
        printf("1. Caută după titlu\n");
        printf("2. Caută după autor\n");
        printf("3. Afișează cărți disponibile\n");
        printf("4. Înapoi la meniul principal\n");
        printf("Alege o opțiune: ");
        
        int opt;
        scanf("%d", &opt);
        getchar(); // consumăm '\n'
        
        clearScreen();
        switch (opt) {
            case 1:
                printf("===== Căutare după titlu =====\n");
                cautaDupaTitlu();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 2:
                printf("===== Căutare după autor =====\n");
                cautaDupaAutor();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 3:
                printf("===== Cărți disponibile =====\n");
                afiseazaCartiDisponibile();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 4:
                // revenim la meniul principal
                return;
            default:
                printf("Opțiune invalidă! Reîncercați.\n");
                printf("Apăsați Enter pentru a continua...");
                getchar();
                break;
        }
    }
}

int main() {

    incarcaDateDinFisier();
    
    while (1) {
        clearScreen();
        printf("===== Meniu Principal =====\n");
        printf("1. Adaugă o carte\n");
        printf("2. Caută cărți (submeniu)\n");
        printf("3. Împrumută carte\n");
        printf("4. Returnează carte\n");
        printf("5. Afișează toate cărțile\n");
        printf("6. Ieșire\n");
        printf("Alege o opțiune: ");
        
        int opt;
        scanf("%d", &opt);
        getchar(); // eliminăm caracterul newline
        
        clearScreen();
        switch (opt) {
            case 1:
                printf("===== Adaugă o nouă carte =====\n");
                adaugaCarte();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 2:
                printf("===== Submeniu Căutare =====\n");
                submeniuCautare();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 3:
                printf("===== Împrumută carte =====\n");
                imprumutaCarte();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 4:
                printf("===== Returnează carte =====\n");
                returneazaCarte();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 5:
                printf("===== Toate cărțile =====\n");
                afiseazaToateCartile();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 6:
                // salveaza date si iesire din aplicatie
                salveazaDateInFisier();
                printf("Aplicația se va închide.\n");
                return 0;
            default:
                printf("Opțiune invalidă! Reîncercați.\n");
                printf("Apăsați Enter pentru a continua...");
                getchar();
                break;
        }
    }
    
    return 0;
}