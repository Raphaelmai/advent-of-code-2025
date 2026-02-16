#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int main() {
    clock_t debut, fin;
    double temps;
    debut = clock();
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    
    char buffer[256];
    int score = 50;
    int total = 0;
    
    while (fgets(buffer, sizeof(buffer), f)) {
        int i = 0;
        int amount = 0;
        
        while (buffer[i] != '\0' && buffer[i] != '\n' && buffer[i] != ' ') {
            i++;
        }
        for (int n = 1; n < i; n++) {
            amount = amount * 10 + (buffer[n] - '0');
        }
        
        char dir = buffer[0];

        for (int step = 0; step < amount; step++) {
            if (dir == 'L') {
                score--;
                if (score < 0)
                    score = 99;
            } else {
                score++;
                if (score > 99)
                    score = 0;
            }

            if (score == 0)
                total++;
        }
    }
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);    
    printf("The actual password to open the door is : %d\n", total);
    printf("Temps d'exécution : %.6f secondes\n", temps);
    return 0;
}
