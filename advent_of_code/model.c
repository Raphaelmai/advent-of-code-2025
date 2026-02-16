#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    clock_t debut, fin;
    double temps;
    debut = clock();
    int total = 0;
    int total2 = 0;
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), f)) {
        
    }
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);    
    printf("Total : %d\n", total);
    printf("Total 2 : %d\n", total2);
    printf("Temps d'exécution : %.3f secondes\n", temps);
    
    return 0;
}