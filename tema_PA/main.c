#include <stdio.h>  /// Bibliotecă standard de intrare/ieșire
#include <stdlib.h> /// Bibliotecă standard pentru funcții de alocare de memorie și generare de numere aleatorii
#include <time.h>   /// Bibliotecă pentru manipularea timpului

#define MAX_SIZE 40        /// Dimensiunea maximă a unui homar
#define MAX_VALUE 500      /// Valoarea maximă a unui homar
#define MAX_NO_HOMARI 100000 /// Numărul maxim de homari permis


/*  Problema cu Homarii
    Cherciu Dumitru-Adrian CR1.1A Proiectarea Algoritmilor */

/// Funcția max pentru a găsi valoarea maximă dintre două numere
int max(int a, int b) {
    return (a > b) ? a : b;
}

/// Funcția knapsack pentru a calcula valoarea maximă
int **knapsack(int capacity, int sizes[], int values[], int numDeHomari, int *max_value) {
    /// Alocă memorie pentru matricea dp
    int **dp = (int **)malloc((numDeHomari + 1) * sizeof(int *));
    for (int i = 0; i <= numDeHomari; i++) {
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));
    }

    /// Inițializează matricea dp
    for (int i = 0; i <= numDeHomari; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0; /// Valoarea este 0 dacă nu avem homari sau capacitate
            } else if (sizes[i - 1] <= w) {
                /// Calculăm valoarea maximă dintre includerea homarului curent și excluderea acestuia
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - sizes[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w]; /// Nu includem homarul curent
            }
        }
    }

    *max_value = dp[numDeHomari][capacity]; /// Valoarea maximă obținută
    return dp;
}

int main() {
    int numDeHomari; /// Numărul de homari
    printf("Introduceti numarul de Homari: "); /// Cere utilizatorului să introducă numărul de homari
    scanf("%d", &numDeHomari); /// Citește numărul de homari de la tastatură

    /// Folosește un fișier de ieșire pentru a afișa informațiile
    FILE *outputFile = fopen("output.txt", "w"); /// Deschide fișierul de ieșire pentru scriere
    if (outputFile == NULL) { /// Verifică dacă fișierul s-a deschis corect
        printf("Eroare, fisierul nu se poate deschide.\n");
        return -1; /// Termină programul dacă fișierul nu a putut fi deschis
    }

    /// Verifică dacă numărul de homari este un număr pozitiv
    if (numDeHomari < 0) {
        fprintf(outputFile, "Numarul de Homari trebuie sa fie pozitiv!\n");
        fclose(outputFile); /// Închide fișierul de ieșire
        return -1; /// Termină programul
    }

    /// Afișează eroare dacă numărul de homari depășește limita maximă permisă
    if (numDeHomari > MAX_NO_HOMARI) {
        fprintf(outputFile, "Prea multi Homari! (maxim %d).\n", MAX_NO_HOMARI);
        fclose(outputFile); /// Închide fișierul de ieșire
        return -1; /// Termină programul
    }

    /// Declară un vector pentru dimensiunile homarilor
    int *sizes = (int *)malloc(numDeHomari * sizeof(int));
    /// Declară un vector pentru valorile homarilor
    int *values = (int *)malloc(numDeHomari * sizeof(int));
    /// Verifică dacă alocarea memoriei a eșuat
    if (sizes == NULL || values == NULL) {
        printf("Alocare de memorie esuata!\n");
        free(sizes); /// Eliberează memoria alocată anterior
        free(values);
        fclose(outputFile); /// Închide fișierul de ieșire
        return -1; /// Termină programul
    }

    srand(time(NULL)); /// Initializează generatorul de numere aleatorii

    /// Generează dimensiuni și valori aleatorii pentru homari
    fprintf(outputFile, "Dimensiunile si Valorile Homarilor:\n");
    for (int i = 0; i < numDeHomari; i++) {
        sizes[i] = rand() % MAX_SIZE + 1; /// Dimensiunea homarului între 1 și MAX_SIZE
        values[i] = rand() % MAX_VALUE + 1; /// Valoarea homarului între 1 și MAX_VALUE
        fprintf(outputFile, "Homar %d: Dimensiune = %d cm, Valoare = %d Monede De Aur\n", i + 1, sizes[i], values[i]);
    }

    int capacity; /// Capacitatea plasei
    printf("Introduceti Capacitatea plasei de pescuit: "); /// Cere utilizatorului să introducă capacitatea plasei
    scanf("%d", &capacity); /// Citește capacitatea plasei de la tastatură
    fprintf(outputFile, "\n");
    fprintf(outputFile, "Capacitatea plasei este %d\n", capacity);
    fprintf(outputFile, "\n");

    /// Afișează eroare dacă capacitatea este negativă
    if (capacity < 0) {
        fprintf(outputFile, "Capacitatea trebuie sa fie un numar natural (in cm!)");
        free(sizes); /// Eliberează memoria alocată pentru dimensiuni
        free(values); /// Eliberează memoria alocată pentru valori
        fclose(outputFile); /// Închide fișierul de ieșire
        return -1; /// Termină programul
    }

    int max_value; /// Valoarea maximă
    /// Calculează valoarea maximă folosind funcția knapsack
    int **dp = knapsack(capacity, sizes, values, numDeHomari, &max_value);

    /// Afișează valoarea maximă în fișierul de ieșire
    fprintf(outputFile, "Valoarea maxima este: %d\n", max_value);
    fprintf(outputFile, "\n");

    /// Calculează și afișează combinația favorabilă de homari care maximizează valoarea totală
    fprintf(outputFile, "Homarii selectati sunt:\n");
    int currentCapacity = capacity; /// Capacitatea curentă a plasei
    for (int i = numDeHomari; i > 0 && max_value > 0; i--) {
        if (dp[i][currentCapacity] != dp[i - 1][currentCapacity]) {
            fprintf(outputFile, "Homar %d: Dimensiune = %d cm, Valoare = %d Monede De Aur\n", i, sizes[i - 1], values[i - 1]);
            currentCapacity -= sizes[i - 1]; /// Scade dimensiunea homarului din capacitatea curentă
            max_value -= values[i - 1]; /// Scade valoarea homarului din valoarea maximă
        }
    }

    fclose(outputFile); /// Închide fișierul de ieșire

    /// Eliberează memoria alocată
    for (int i = 0; i <= numDeHomari; i++) {
        free(dp[i]); /// Eliberează fiecare rând din matricea dp
    }
    free(dp); /// Eliberează matricea dp
    free(sizes); /// Eliberează vectorul sizes
    free(values); /// Eliberează vectorul values

    return 0; /// Termină programul cu succes
}

