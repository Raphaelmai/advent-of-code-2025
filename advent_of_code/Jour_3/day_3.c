#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
void max_digits(const char *num, int k, char *out) {
    int n = strlen(num);
    int start = 0;
    int out_pos = 0;

    while (k > 0) {
        int end = n - k;
        char max_digit = '0';
        int max_pos = start;

        for (int i = start; i < end; i++) {
            if (num[i] > max_digit) {
                max_digit = num[i];
                max_pos = i;
            }
        }

        out[out_pos++] = max_digit;
        k--;
        start = max_pos + 1;
    }

    out[out_pos] = '\0';
}


int main() {
    clock_t debut, fin;
    double temps;
    debut = clock();
    FILE *f = fopen("test.txt", "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    char buffer[256];
    int total = 0;
    long long int total2;
    while (fgets(buffer, sizeof(buffer), f)) {
    // Part 1    
        char result1[20];
        max_digits(buffer, 2, result1);
        total += atoi(result1);
    // Part 2
        char result[20];
        max_digits(buffer, 12, result);

        total2 += atoll(result);
    }
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);    
    printf("The total output joltage is %d\n", total);
    printf("The new total output joltage is %lld\n", total2);
    printf("Temps d'exécution : %.6f secondes\n", temps);
    return 0;
}
