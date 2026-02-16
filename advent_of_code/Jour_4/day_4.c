#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX_LIGNES 136
#define MAX_LONGUEUR 200
int main() {
    clock_t debut, fin;
    double temps;
    debut = clock();
    char lignes[MAX_LIGNES][MAX_LONGUEUR];
    int nb_lignes = 0;
    FILE *fichier = fopen("input.txt", "r");
    if (!fichier) return 1;
    while (nb_lignes < MAX_LIGNES && fgets(lignes[nb_lignes], MAX_LONGUEUR, fichier)) {
        lignes[nb_lignes][strcspn(lignes[nb_lignes], "\n")] = '\0';
        nb_lignes++;
    }
    fclose(fichier);
    int counter = 1;
    int counter2 = 0;
    int index = 0;
    while (counter != 0){
        counter = 0;
        for (int i = 0; i < nb_lignes; i++) {
            for (int j = 0; j < MAX_LONGUEUR; j++) {
                if (lignes[i][j] == '\0') break;
                if (lignes[i][j] == '@') {
                    int k = 0;
                    if (j+1 < strlen(lignes[i]) && lignes[i][j+1] == '@') k++;
                    if (j-1 >= 0 && lignes[i][j-1] == '@') k++;
                    if (i-1 >= 0 && j < strlen(lignes[i-1]) && lignes[i-1][j] == '@') k++;
                    if (i+1 < nb_lignes && j < strlen(lignes[i+1]) && lignes[i+1][j] == '@') k++;
                    if (i-1 >= 0 && j-1 >= 0 && j-1 < strlen(lignes[i-1]) && lignes[i-1][j-1] == '@') k++;
                    if (i-1 >= 0 && j+1 < strlen(lignes[i-1]) && lignes[i-1][j+1] == '@') k++;
                    if (i+1 < nb_lignes && j-1 >= 0 && j-1 < strlen(lignes[i+1]) && lignes[i+1][j-1] == '@') k++;
                    if (i+1 < nb_lignes && j+1 < strlen(lignes[i+1]) && lignes[i+1][j+1] == '@') k++;
                    if (k < 4) {
                        counter++;
                        lignes[i][j] = 'x';
                    }
                }
            }
        }
        if (index ==0) {
        printf ("%d rolls of paper can be accessed by a forklift\n", counter);
        index++;
        }
        counter2 += counter;
        
    }
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    printf("%d rolls of paper in total can be removed by the Elves and their forklifts\n", counter2);
    printf("Temps d'exécution : %.3f secondes\n", temps);
    return 0;
}
