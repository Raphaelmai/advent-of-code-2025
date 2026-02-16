#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINES 200
#define MAX_PARENS 30
#define MAX_COUNTERS 30

static int bitcount(int x){
    int c = 0;
    while(x){ c += x&1; x >>= 1; }
    return c;
}

int lowest_click(char tabBrackets[MAX_LINES][256], char tabParens[MAX_LINES][MAX_PARENS][256], int nbParens[MAX_LINES], int L){
    int target = 0;
    int len = 0;
    for(int i=0; tabBrackets[L][i]; i++){
        char c = tabBrackets[L][i];
        if(c=='.' || c=='#'){
            if(c=='#') target |= (1 << len);
            len++;
        }
    }

    int nbB = nbParens[L];
    int buttonMask[MAX_PARENS];
    for(int b=0; b<nbB; b++){
        int m = 0;
        int idx = 0;
        char *p = tabParens[L][b];
        while(*p){
            if(*p >= '0' && *p <= '9'){
                idx = idx*10 + (*p - '0');
            } else {
                m |= (1 << idx);
                idx = 0;
            }
            p++;
        }
        m |= (1 << idx);
        buttonMask[b] = m;
    }

    int best = 1000000;
    int total = 1 << nbB;
    for(int mask=0; mask<total; mask++){
        int presses = bitcount(mask);
        if(presses >= best) continue;
        int cur = 0;
        for(int b=0; b<nbB; b++){
            if(mask & (1<<b)){
                cur ^= buttonMask[b];
            }
        }
        if(cur == target){
            best = presses;
        }
    }
    return best;
}

// Variables globales pour l'optimisation
int g_bestSolution;
int g_buttonEffect[MAX_PARENS][MAX_COUNTERS];
int g_target[MAX_COUNTERS];
int g_nbButtons;
int g_nbCounters;

// Recherche récursive avec élagage (branch and bound)
void searchMinPresses(int buttonIdx, int currentPresses, int presses[MAX_PARENS], int counters[MAX_COUNTERS]){
    // Élagage: si on a déjà dépassé la meilleure solution
    if(currentPresses >= g_bestSolution) return;
    
    // Si on a testé tous les boutons
    if(buttonIdx == g_nbButtons){
        // Vérifier si c'est une solution valide
        int valid = 1;
        for(int c = 0; c < g_nbCounters; c++){
            if(counters[c] != g_target[c]){
                valid = 0;
                break;
            }
        }
        
        if(valid && currentPresses < g_bestSolution){
            g_bestSolution = currentPresses;
        }
        return;
    }
    
    // Calculer une borne inférieure heuristique
    int remaining[MAX_COUNTERS];
    int maxRemaining = 0;
    for(int c = 0; c < g_nbCounters; c++){
        remaining[c] = g_target[c] - counters[c];
        if(remaining[c] > maxRemaining) maxRemaining = remaining[c];
    }
    
    // Si borne inférieure dépasse meilleure solution, élaguer
    if(currentPresses + maxRemaining >= g_bestSolution) return;
    
    // Essayer différents nombres de pressions pour ce bouton
    // Limite intelligente basée sur les valeurs restantes
    int maxPress = maxRemaining + 5;
    if(maxPress > 50) maxPress = 50;
    
    for(int press = 0; press <= maxPress; press++){
        // Ne pas presser ce bouton
        if(press == 0){
            searchMinPresses(buttonIdx + 1, currentPresses, presses, counters);
        }
        else {
            // Presser ce bouton 'press' fois
            int newCounters[MAX_COUNTERS];
            memcpy(newCounters, counters, sizeof(newCounters));
            
            int valid = 1;
            for(int c = 0; c < g_nbCounters; c++){
                if(g_buttonEffect[buttonIdx][c]){
                    newCounters[c] += press;
                    // Élagage: si on dépasse la cible
                    if(newCounters[c] > g_target[c]){
                        valid = 0;
                        break;
                    }
                }
            }
            
            if(valid){
                int newPresses[MAX_PARENS];
                memcpy(newPresses, presses, sizeof(int) * MAX_PARENS);
                newPresses[buttonIdx] = press;
                
                searchMinPresses(buttonIdx + 1, currentPresses + press, newPresses, newCounters);
            } else {
                break; // Inutile d'essayer plus de pressions
            }
        }
    }
}

int lowest_click_joltage(char tabParens[MAX_LINES][MAX_PARENS][256], int nbParens[MAX_LINES], 
                         int tabBraces[MAX_LINES][MAX_PARENS], int nbBraces[MAX_LINES], int L){
    g_nbCounters = nbBraces[L];
    g_nbButtons = nbParens[L];
    
    if(g_nbCounters == 0) return 0;
    
    // Construire la matrice des effets
    memset(g_buttonEffect, 0, sizeof(g_buttonEffect));
    
    for(int b = 0; b < g_nbButtons; b++){
        char *p = tabParens[L][b];
        int idx = 0;
        while(*p){
            if(*p >= '0' && *p <= '9'){
                idx = idx*10 + (*p - '0');
            } else {
                if(idx < g_nbCounters){
                    g_buttonEffect[b][idx] = 1;
                }
                idx = 0;
            }
            p++;
        }
        if(idx < g_nbCounters){
            g_buttonEffect[b][idx] = 1;
        }
    }
    
    // Valeurs cibles
    int maxTarget = 0;
    for(int i = 0; i < g_nbCounters; i++){
        g_target[i] = tabBraces[L][i];
        if(g_target[i] > maxTarget) maxTarget = g_target[i];
    }
    
    // Initialiser la recherche
    g_bestSolution = maxTarget * g_nbCounters; // Borne supérieure naïve
    
    int presses[MAX_PARENS] = {0};
    int counters[MAX_COUNTERS] = {0};
    
    // Lancer la recherche
    searchMinPresses(0, 0, presses, counters);
    
    return (g_bestSolution >= maxTarget * g_nbCounters) ? -1 : g_bestSolution;
}

int main(int argc, char *argv[]) {
    clock_t debut, fin;
    double temps;
    debut = clock();
    int total = 0;
    int total2 = 0;
    
    const char *filename = (argc > 1) ? argv[1] : "test.txt";
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }
    char line[512];

    char tabBrackets[MAX_LINES][256];
    char tabParens[MAX_LINES][MAX_PARENS][256];
    int  nbParens[MAX_LINES];
    int tabBraces[MAX_LINES][MAX_PARENS];
    int nbBraces[MAX_LINES];

    int nlines = 0;

    while(fgets(line,sizeof line,f) && nlines < MAX_LINES){

        sscanf(line," [%[^]]]", tabBrackets[nlines]);
        nbParens[nlines] = 0;
        char *p = line;
        while((p = strchr(p,'(')) != NULL){
            p++;
            int i=0;
            while(*p && *p!=')' && i<255){
                tabParens[nlines][nbParens[nlines]][i++] = *p++;
            }
            tabParens[nlines][nbParens[nlines]][i]='\0';
            nbParens[nlines]++;
        }

        char *a = strchr(line,'{');
        nbBraces[nlines] = 0;

        if(a){
            a++;
            while(*a && *a != '}'){
                if(*a >= '0' && *a <= '9'){
                    int val = 0;
                    while(*a >= '0' && *a <= '9'){
                        val = val*10 + (*a - '0');
                        a++;
                    }
                    tabBraces[nlines][nbBraces[nlines]++] = val;
                } else {
                    a++;
                }
            }
        }

        nlines++;
    }

    for(int L=0; L<nlines; L++) {
        total += lowest_click(tabBrackets, tabParens, nbParens, L);
        int result = lowest_click_joltage(tabParens, nbParens, tabBraces, nbBraces, L);
        if(result != -1){
            total2 += result;
        }
    }

    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);
    printf("The fewest button presses required to correctly configure the indicator lights on all of the machines is %d\n", total);
    printf("The fewest button presses required to correctly configure the joltage level counters on all of the machines is %d\n", total2);
    printf("Temps d'exécution : %.3f secondes\n", temps);

    return 0;
}