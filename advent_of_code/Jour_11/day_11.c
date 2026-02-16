#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 1000
#define MAX_NEIGHBORS 50
#define MAX_NAME 20
#define MAX_VISITED 100

typedef struct {
    char name[MAX_NAME];
    char neighbors[MAX_NEIGHBORS][MAX_NAME];
    int nb_neighbors;
} Node;

Node nodes[MAX_NODES];
int nb_nodes = 0;

// Trouve l'index d'un nœud par son nom, ou retourne -1 si non trouvé
int trouver_noeud(char *name) {
    for (int i = 0; i < nb_nodes; i++) {
        if (strcmp(nodes[i].name, name) == 0) return i;
    }
    return -1;
}

// Ajoute un nœud s'il n'existe pas déjà, et retourne son index
int ajouter_noeud(char *name) {
    int idx = trouver_noeud(name);
    if (idx != -1) return idx;
    strcpy(nodes[nb_nodes].name, name);
    nodes[nb_nodes].nb_neighbors = 0;
    return nb_nodes++;
}

// Vérifie si un nœud a déjà été visité dans le chemin actuel
int est_visite(char visited[][MAX_NAME], int nb_visited, char *name) {
    for (int i = 0; i < nb_visited; i++) {
        if (strcmp(visited[i], name) == 0) return 1;
    }
    return 0;
}

// Fonction de recherche en profondeur pour la Partie 1
int explorer_p1(char *cur, char *end, char visited[][MAX_NAME], int nb_visited) {
    // Si on a atteint la destination, on a trouvé un chemin
    if (strcmp(cur, end) == 0) return 1;
    
    // Si le nœud a déjà été visité dans ce chemin, pas de chemin valide
    if (est_visite(visited, nb_visited, cur)) return 0;
    
    // Trouver le nœud dans le graphe
    int idx = trouver_noeud(cur);
    if (idx == -1) return 0;
    
    // Marquer le nœud comme visité
    strcpy(visited[nb_visited], cur);
    
    // Explorer tous les voisins
    int total = 0;
    for (int i = 0; i < nodes[idx].nb_neighbors; i++) {
        total += explorer_p1(nodes[idx].neighbors[i], end, visited, nb_visited + 1);
    }
    
    return total;
}

// Fonction de recherche en profondeur pour la Partie 2
// Trouve les chemins qui passent par TOUS les nœuds requis
int explorer_p2(char *cur, char *end, char visited[][MAX_NAME], int nb_visited,
                char required[][MAX_NAME], int nb_required, int *visited_required) {
    int mark = -1;
    
    // Marquer le nœud courant comme visité s'il est dans la liste des requis
    for (int i = 0; i < nb_required; i++) {
        if (strcmp(cur, required[i]) == 0 && !visited_required[i]) {
            visited_required[i] = 1;
            mark = i;
        }
    }

    // Si on est arrivé à la fin, vérifier que tous les nœuds requis ont été visités
    if (strcmp(cur, end) == 0) {
        for (int i = 0; i < nb_required; i++) {
            if (!visited_required[i]) {
                // Un nœud requis n'a pas été visité, ce chemin n'est pas valide
                if (mark != -1) visited_required[mark] = 0; // Backtracking
                return 0;
            }
        }
        // Tous les nœuds requis ont été visités, ce chemin est valide !
        if (mark != -1) visited_required[mark] = 0; // Backtracking
        return 1;
    }

    // Si le nœud a déjà été visité dans ce chemin, pas de chemin valide
    if (est_visite(visited, nb_visited, cur)) {
        if (mark != -1) visited_required[mark] = 0; // Backtracking
        return 0;
    }

    // Trouver l'index du nœud courant
    int idx = trouver_noeud(cur);
    if (idx == -1) {
        if (mark != -1) visited_required[mark] = 0; // Backtracking
        return 0;
    }

    // Ajouter le nœud courant à la liste des visités
    strcpy(visited[nb_visited], cur);
    int total = 0;

    // Explorer tous les voisins
    for (int i = 0; i < nodes[idx].nb_neighbors; i++) {
        total += explorer_p2(nodes[idx].neighbors[i], end, visited, nb_visited + 1,
                             required, nb_required, visited_required);
    }

    // Backtracking : démarquer le nœud requis si nécessaire
    if (mark != -1) visited_required[mark] = 0;
    return total;
}

// Lit le fichier d'entrée et construit le graphe
void lire_fichier(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { 
        printf("Erreur: impossible d'ouvrir %s\n", filename); 
        exit(1); 
    }
    
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        // Retirer le retour à la ligne
        ligne[strcspn(ligne, "\n")] = 0;
        if (strlen(ligne) == 0) continue;
        
        // Séparer le nom du nœud et ses voisins
        char *token = strtok(ligne, ":");
        if (!token) continue;
        
        char nom_noeud[MAX_NAME];
        strcpy(nom_noeud, token);
        
        // Retirer les espaces au début
        while (*nom_noeud == ' ') memmove(nom_noeud, nom_noeud + 1, strlen(nom_noeud));
        
        int idx = ajouter_noeud(nom_noeud);
        
        // Lire les voisins
        token = strtok(NULL, ":");
        if (!token) continue;
        
        char *voisin = strtok(token, " ");
        while (voisin) {
            if (strlen(voisin) > 0) {
                strcpy(nodes[idx].neighbors[nodes[idx].nb_neighbors], voisin);
                nodes[idx].nb_neighbors++;
            }
            voisin = strtok(NULL, " ");
        }
    }
    fclose(f);
}

int main() {
    lire_fichier("input.txt");
    
    char visited[MAX_VISITED][MAX_NAME];


    int resultat_p1 = explorer_p1("you", "out", visited, 0);
    printf("%d paths found\n", resultat_p1);

    return 0;
}