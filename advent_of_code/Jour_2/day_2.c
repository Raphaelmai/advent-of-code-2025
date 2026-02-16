#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int repeated(long long int x){
    char s[32];
    int length = snprintf(s, sizeof(s), "%lld", x);
    if (length < 0) return 0;

    if (length%2 != 0) return 0;
        int tab[length];
        for (int i = 0 ; i < length; i++){
            tab[i] = x%10;
            x/=10;
        }

        for (int i = 0 ; i < length/2; i++){
            if (tab[i] != tab[i + length/2]) return 0;
        }
    
    return 1;
}

int repeated_more(long long int x){
    char s[32];
    int n = snprintf(s, sizeof(s), "%lld", x);
    if (n < 0) return 0;

    for (int l = 1; l <= n/2; l++) {  
        if (n % l != 0) continue;     
        int valid = 1;
        for (int i = 0; i < n; i++) {
            if (s[i] != s[i % l]) {
                valid = 0;
                break;
            }
        }
        if (valid) return 1; 
    }
    return 0;
}



int main() {
    clock_t debut, fin;
    double temps;
    debut = clock();
    FILE *f = fopen("input.txt", "r");
    if (!f) return 1;
    long long int total = 0;
    long long int total2 = 0;
    char buffer[1000];
    if (!fgets(buffer, sizeof(buffer), f)) return 1;

    char *token = strtok(buffer, ",");
    while (token != NULL) {

        long long int a, b;
        if (sscanf(token, "%lld-%lld", &a, &b) == 2) {
            for (long long int i = a ; i <=b ; i++){
                if (repeated_more(i)){
                    total2 += i;
                }
                if (repeated(i)){
                    total += i;
                }
            }
        }

        token = strtok(NULL, ",");
    }
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);
    printf("If we add up all the invalid ID's, we get %lld\n", total);
    printf("Using these new rules we get %lld\n", total2);
    printf("Temps d'exécution : %.3f secondes\n", temps);
    return 0;
}
