#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int locate_beamer(char tab[150][200]) {
    int i = 0;
    while (tab[0][i] != 'S') {
        i++;
    }
    return i;
}

int split_amount(char tab[150][200], int lines) {
    int total = 0;
    int split_pos = locate_beamer(tab);
    tab[0][split_pos] = '|';
    for (int i = 1; i < lines; i++) {
        int len = strlen(tab[i]);
        for (int j = 0; j < len; j++) {
            if (tab[i-1][j] == '|') {
                if (tab[i][j] == '^') {
                    tab[i][j-1] = '|';
                    tab[i][j+1] = '|';
                    total++;
                } else {
                    tab[i][j] = '|';
                }
            }
        }
        
    }
    return total;
}

long long int quantum_split(char tab[150][200], int lines) {
    long long int total = 0;
    long long int paths[150][200] = {0};
    int split_pos = locate_beamer(tab);
    tab[0][split_pos] = '|';
    paths[0][split_pos] = 1;
    for (int i = 1; i < lines; i++) {
        int len = strlen(tab[i]);
        for (int j = 0; j < len; j++) {
            if (tab[i-1][j] == '|') {
                if (tab[i][j] == '^') {
                    tab[i][j-1] = '|';
                    tab[i][j+1] = '|';
                    paths[i][j-1] += paths[i-1][j];
                    paths[i][j+1] += paths[i-1][j];
                    
                } else {
                    tab[i][j] = '|';
                    paths[i][j] += paths[i-1][j];
                }
            }
        }
        
    }
    for (int j = 0; j < 200; j++) {
        total += paths[lines-1][j];
    }
    return total;
}

int main(int argc, char *argv[]) {
    clock_t debut, fin;
    double temps;
    debut = clock();
    int total = 0;
    long long int total2 = 0;
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    char buffer[256];
    char tab[150][200];
    char tab2[150][200];

    int i = 0;
    while (fgets(buffer, sizeof(buffer), f)) {
        strcpy(tab[i], buffer);
        strcpy(tab2[i], buffer);
        i++;
    }
    total = split_amount(tab, i);
    total2 = quantum_split(tab2, i);
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);    
    printf("The beam will split %d times\n", total);
    printf("This tachyon could take a total of %lld different timelines\n", total2);
    printf("Temps d'exécution : %.6f secondes\n", temps);
    
    return 0;
}
