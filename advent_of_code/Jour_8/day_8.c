#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    long long int dist;
    int i, j;
} edge;

long long int distance(long long int x1, long long int y1, long long int z1, long long int x2, long long int y2, long long int z2) {
    return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
}

int array[2000];

void start_union(int n) {
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }
}

int find(int x) {
    if (array[x] != x) {
        array[x] = find(array[x]);
    }
    return array[x];
}

void unite(int x, int y) {
    int px = find(x);
    int py = find(y);
    if (px != py) {
        array[px] = py;
    }
}

int compare_edges(const void *a, const void *b) {
    edge *ea = (edge *)a;
    edge *eb = (edge *)b;
    if (ea->dist < eb->dist) return -1;
    if (ea->dist > eb->dist) return 1;
    return 0;
}

int group(int tab[][2], int k, int n) {
    start_union(n);
    
    for (int i = 0; i < k; i++) {
        unite(tab[i][0], tab[i][1]);
    }
    
    int circuit_size[2000] = {0};
    for (int i = 0; i < n; i++) {
        circuit_size[find(i)]++;
    }
    
    int a = 0, b = 0, c = 0;
    for (int i = 0; i < n; i++) {
        int x = circuit_size[i];
        if (x > a) {
            c = b;
            b = a;
            a = x;
        } else if (x > b) {
            c = b;
            b = x;
        } else if (x > c) {
            c = x;
        }
    }
    
    return a * b * c;
}

int part1(long long int tab[][3], int n) {
    static edge edges[500000];
    
    int idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges[idx].dist = distance(tab[i][0], tab[i][1], tab[i][2], 
                                      tab[j][0], tab[j][1], tab[j][2]);
            edges[idx].i = i;
            edges[idx].j = j;
            idx++;
        }
    }
    
    qsort(edges, idx, sizeof(edge), compare_edges);
    int shortest_pair[1000][2];
    for (int k = 0; k < 1000; k++) {
        shortest_pair[k][0] = edges[k].i;
        shortest_pair[k][1] = edges[k].j;
    }
    return group(shortest_pair, 1000, n);
}

int all_connected(int n){
    int r=find(0);
    for(int i=1;i<n;i++){
        if(find(i)!=r) return 0;
    }
    return 1;
}

long long int part2(long long int tab[][3], int n){
    static edge edges[500000];
    
    int num_edges = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges[num_edges].dist = distance(tab[i][0], tab[i][1], tab[i][2],
                                             tab[j][0], tab[j][1], tab[j][2]);
            edges[num_edges].i = i;
            edges[num_edges].j = j;
            num_edges++;
        }
    }
    
    qsort(edges, num_edges, sizeof(edge), compare_edges);
    start_union(n);
    
    int a = 0, b = 0;
    for (int k = 0; k < num_edges; k++) {
        int i = edges[k].i;
        int j = edges[k].j;
        
        if (find(i) != find(j)) {
            unite(i, j);
            a = i;
            b = j;
            
            if (all_connected(n)) {
                break;
            }
        }
    }
    
    return tab[a][0] * tab[b][0];
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
    long long int tab[1000][3];
    int i = 0;
    long long int x, y, z;
    while (fgets(buffer, sizeof(buffer), f)) {
        if (sscanf(buffer, "%lld,%lld,%lld", &tab[i][0], &tab[i][1], &tab[i][2]) == 3) i++;
    }

    total = part1(tab, i);
    total2 = part2(tab, i);
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);    
    printf("If you multiply together the sizes of the three largest circuits, we get %d\n", total);
    printf("If you multiply together the X coordinates of the last two junction boxes you need to connect we get %lld\n", total2);
    printf("Temps d'exécution : %.3f secondes\n", temps);
    
    return 0;
}