#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

long long int calculator(char tab[100][10000][24], int lines, int columns) {
    long long int total = 0;
    for (int j = 0; j < columns; j++) {
        if (tab[lines - 1][j][0] == '\0') {
            continue;
        }
        
        long long int subtotal = 0;
        switch (tab[lines - 1][j][0]) {
            case '+':
                for (int k = 0; k < lines - 1; k++) {
                    subtotal += atoll(tab[k][j]);
                }
                break;
            case '*':
                subtotal = 1;
                for (int k = 0; k < lines - 1; k++) {
                    subtotal *= atoll(tab[k][j]);
                }
                break;
            default:
                break;
        }
        total += subtotal;
    }
    return total;
}

long long int calculator_remake(char tab[100][10000], int lines, int columns) {
    long long int total = 0;
    long long int sum = 0;
    long long int product = 1;
    char op = '+';
    for (int col = 0; col < columns; col++) {
        long long int subtotal = 0;
        if (tab[lines - 1][col] == '*' || tab[lines - 1][col] == '+') {
            if (op == '+') total  += sum;
            else if (op == '*') total += product;
            product = 1;
            sum = 0;
        }
        if (tab[lines - 1][col] == '*') op = '*';
        else if (tab[lines - 1][col] == '+') op = '+';

        if (op == '+') {
            char buffer[32] = "";
            for (int row = 0; row < lines - 1; row++) {
                if (tab[row][col] >= '0' && tab[row][col] <= '9') {
                    int len = strlen(buffer);
                    buffer[len] = tab[row][col];
                    buffer[len+1] = '\0';
                }
            }
            if (buffer[0] != '\0'){
                sum += atoll(buffer);
                
            }
            
        }
        else if (op == '*') {
            long long value = 1;
            char buffer[32] = "";
            for (int row = 0; row < lines - 1; row++) {
                if (tab[row][col] >= '0' && tab[row][col] <= '9') {
                    int len = strlen(buffer);
                    buffer[len] = tab[row][col];
                    buffer[len+1] = '\0';
                }
            }
            if (buffer[0] != '\0'){
                product *= atoll(buffer);
                
            }
        }
    }
    if (op == '+') total += sum;
    else if (op == '*') total += product;
    return total;
}


int main(int argc, char *argv[]) {
    clock_t debut, fin;
    double temps;
    debut = clock();
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }
    
    char buffer[100000];
    static char tab[100][10000][24];
    char tab2[100][10000];
    int i = 0, max_j = 0;
    
    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(tab2[i], buffer);
        int j = 0;
        int pos = 0;
        int len = strlen(buffer);
        
        while (pos < len) {
            while (pos < len && buffer[pos] == ' ') {
                pos++;
            }
            
            if (pos >= len) break;
            
            int start = pos;
            while (pos < len && buffer[pos] != ' ') {
                pos++;
            }
            
            int token_len = pos - start;
            strncpy(tab[i][j], buffer + start, token_len);
            tab[i][j][token_len] = '\0';
            j++;
        }
        
        if (j > max_j) max_j = j;
        i++;
    }
    
    fclose(f);
    
    long long int x = calculator(tab, i, max_j);
    long long int y = calculator_remake(tab2, i, strlen(tab2[0]));
    
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    
    printf("The grand total found by adding together all of the answers to the individual problems is %lld\n", x);
    printf("The new grand total found by adding together all of the answers to the individual problems is %lld\n", y);
    printf("Temps d'exécution : %.6f secondes\n", temps);
    
    return 0;
}