#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIM_CARTI 500
#define FISIER "biblioteca.txt"

// structura carte
typedef struct {
    char titlu[100];
    char autor[100];
    char tip[50];       // Ex: roman, nuvela, poezie etc.
    char locatie[50];   // Ex: raftul X, sala de lectura etc.
    int disponibil;     // 1 = disponibil, 0 = împrumutat
    int zile_ramase;    // numărul de zile ramase daca este imprumutata (max 10)
} Carte;

Carte carti[MAXIM_CARTI];
int nrCarti = 0;

//functie de curatare a ecranului
void clearScreen() {
    system("clear"); // sau cls pe windows
}

void incarcaDateDinFisier() {
    FILE *f = fopen(FISIER, "r");
    if (!f) {
        // Dacă fișierul nu există, nu facem nimic, se va crea ulterior
        return;
    }
    
    nrCarti = 0;
    while (!feof(f)) {
        char linie[256];
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
        fprintf(f, "%s|%s|%s|%s|%d|%d\n",
                carti[i].titlu,
                carti[i].autor,
                carti[i].tip,
                carti[i].locatie,
                carti[i].disponibil,
                carti[i].zile_ramase);
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
        printf("Titlu: %s\n", carti[i].titlu);
        printf("Autor: %s\n", carti[i].autor);
        printf("Tip: %s\n", carti[i].tip);
        printf("Locație: %s\n", carti[i].locatie);
        if (carti[i].disponibil == 1) {
            printf("Stare: Disponibilă\n");
        } else {
            printf("Stare: Împrumutată (%d zile rămase)\n", carti[i].zile_ramase);
        }
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
    
    // se marcheaza cartea ca fiind disponibila
    carti[nrCarti].disponibil = 1;
    carti[nrCarti].zile_ramase = 0;
    
    nrCarti++;
    printf("Cartea a fost adăugată cu succes!\n");
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
                //submeniuCautare();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 3:
                printf("===== Împrumută carte =====\n");
                //imprumutaCarte();
                printf("\nApăsați Enter pentru a continua...");
                getchar();
                break;
            case 4:
                printf("===== Returnează carte =====\n");
                //returneazaCarte();
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