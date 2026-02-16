#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_BTNS 20
#define MAX_CNTS 20
#define LINE_LEN 4096

int n_btns, n_cnts;
int A[MAX_BTNS][MAX_CNTS];
int target[MAX_CNTS];

int best;
int cur[MAX_CNTS];
int max_gain[MAX_CNTS];

/* borne inférieure : minimum de boutons nécessaires */
int lower_bound() {
    int lb = 0;
    for (int i = 0; i < n_cnts; i++) {
        int rem = target[i] - cur[i];
        if (rem > 0) {
            int g = max_gain[i];
            if (g == 0) return INT_MAX;
            int need = (rem + g - 1) / g;
            if (need > lb) lb = need;
        }
    }
    return lb;
}

/* DFS branch & bound */
void dfs(int idx, int used) {
    if (used >= best) return;
    if (used + lower_bound() >= best) return;

    if (idx == n_btns) {
        for (int i = 0; i < n_cnts; i++)
            if (cur[i] != target[i])
                return;
        best = used;
        return;
    }

    int maxk = INT_MAX;
    for (int i = 0; i < n_cnts; i++) {
        if (A[idx][i] > 0) {
            int rem = target[i] - cur[i];
            if (rem < 0) return;
            int k = rem / A[idx][i];
            if (k < maxk) maxk = k;
        }
    }

    /* essayer les gros k d'abord pour réduire best vite */
    for (int k = maxk; k >= 0; k--) {
        for (int i = 0; i < n_cnts; i++)
            cur[i] += k * A[idx][i];

        dfs(idx + 1, used + k);

        for (int i = 0; i < n_cnts; i++)
            cur[i] -= k * A[idx][i];
    }
}

/* résolution d'une machine */
int solve_machine() {
    memset(cur, 0, sizeof(cur));
    best = INT_MAX;

    /* pré-calcul des gains max */
    for (int i = 0; i < n_cnts; i++) {
        max_gain[i] = 0;
        for (int j = 0; j < n_btns; j++)
            if (A[j][i] > max_gain[i])
                max_gain[i] = A[j][i];
    }

    dfs(0, 0);
    return best;
}

int main(int argc, char **argv) {


    FILE *f = fopen("input.txt", "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    char line[LINE_LEN];
    long long total = 0;

    while (fgets(line, sizeof(line), f)) {
        char *p = strchr(line, '{');
        if (!p) continue;

        /* lecture des cibles */
        n_cnts = 0;
        char *q = p + 1;
        while (*q && *q != '}') {
            target[n_cnts++] = strtol(q, &q, 10);
            if (*q == ',') q++;
        }

        /* lecture des boutons */
        n_btns = 0;
        char *s = line;
        while ((s = strchr(s, '('))) {
            memset(A[n_btns], 0, sizeof(A[n_btns]));
            s++;
            while (*s && *s != ')') {
                int id = strtol(s, &s, 10);
                A[n_btns][id]++;
                if (*s == ',') s++;
            }
            n_btns++;
            s++;
        }

        int res = solve_machine();
        total += res;
    }

    fclose(f);

    printf("%lld\n", total);
    return 0;
}
