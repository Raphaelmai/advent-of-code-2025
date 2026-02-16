// Advent of Code 2025 – Day 12
// Christmas Tree Farm
// Brute-force exact cover with backtracking + pruning
// Compile: gcc -O2 day12.c -o day12
// Run: ./day12 input.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SHAPES 6
#define MAX_ORI 8
#define MAX_CELLS 9
#define MAX_W 50
#define MAX_H 50
#define MAX_PIECES 600

typedef struct {
    int n;
    int x[MAX_CELLS];
    int y[MAX_CELLS];
} Shape;

typedef struct {
    int nori;
    Shape ori[MAX_ORI];
    int area;
} ShapeSet;

static ShapeSet shapes[MAX_SHAPES];
static int board[MAX_H][MAX_W];
static int W, H;
static int pieces[MAX_PIECES];
static int npieces;

static int dx[4] = {1, 0, -1, 0};
static int dy[4] = {0, 1, 0, -1};

static int min(int a, int b) { return a < b ? a : b; }

static Shape rotate90(const Shape *s) {
    Shape r;
    r.n = s->n;
    for (int i = 0; i < s->n; i++) {
        r.x[i] = -s->y[i];
        r.y[i] =  s->x[i];
    }
    return r;
}

static Shape flipH(const Shape *s) {
    Shape r;
    r.n = s->n;
    for (int i = 0; i < s->n; i++) {
        r.x[i] = -s->x[i];
        r.y[i] =  s->y[i];
    }
    return r;
}

static void normalize(Shape *s) {
    int minx = s->x[0], miny = s->y[0];
    for (int i = 1; i < s->n; i++) {
        minx = min(minx, s->x[i]);
        miny = min(miny, s->y[i]);
    }
    for (int i = 0; i < s->n; i++) {
        s->x[i] -= minx;
        s->y[i] -= miny;
    }
}

static int same_shape(const Shape *a, const Shape *b) {
    if (a->n != b->n) return 0;
    for (int i = 0; i < a->n; i++) {
        if (a->x[i] != b->x[i] || a->y[i] != b->y[i])
            return 0;
    }
    return 1;
}

static void add_orientation(int id, Shape s) {
    normalize(&s);
    for (int i = 0; i < shapes[id].nori; i++) {
        if (same_shape(&shapes[id].ori[i], &s))
            return;
    }
    shapes[id].ori[shapes[id].nori++] = s;
}

static void build_orientations(int id, Shape base) {
    Shape r = base;
    for (int i = 0; i < 4; i++) {
        add_orientation(id, r);
        Shape f = flipH(&r);
        add_orientation(id, f);
        r = rotate90(&r);
    }
}

static int can_place(const Shape *s, int ox, int oy) {
    for (int i = 0; i < s->n; i++) {
        int x = ox + s->x[i];
        int y = oy + s->y[i];
        if (x < 0 || y < 0 || x >= W || y >= H) return 0;
        if (board[y][x]) return 0;
    }
    return 1;
}

static void place(const Shape *s, int ox, int oy, int v) {
    for (int i = 0; i < s->n; i++)
        board[oy + s->y[i]][ox + s->x[i]] = v;
}

static int solve(int k) {
    if (k == npieces) return 1;
    int id = pieces[k];
    for (int o = 0; o < shapes[id].nori; o++) {
        Shape *s = &shapes[id].ori[o];
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (can_place(s, x, y)) {
                    place(s, x, y, 1);
                    if (solve(k + 1)) return 1;
                    place(s, x, y, 0);
                }
            }
        }
    }
    return 0;
}

static int cmp_area(const void *a, const void *b) {
    int ia = *(int *)a;
    int ib = *(int *)b;
    return shapes[ib].area - shapes[ia].area;
}

int main(int argc, char **argv) {
    FILE *f = fopen("input.txt", "r");
    if (!f) return 1;

    char line[128];
    for (int i = 0; i < MAX_SHAPES; i++) {
        fgets(line, sizeof(line), f);
        fgets(line, sizeof(line), f);
        Shape base = {0};
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (line[x] == '#') {
                    base.x[base.n] = x;
                    base.y[base.n] = y;
                    base.n++;
                }
            }
            fgets(line, sizeof(line), f);
        }
        shapes[i].nori = 0;
        shapes[i].area = base.n;
        build_orientations(i, base);
    }

    int count = 0;
    clock_t start = clock();

    while (fscanf(f, "%dx%d:", &W, &H) == 2) {
        int need[MAX_SHAPES];
        int area = 0;
        npieces = 0;

        for (int i = 0; i < MAX_SHAPES; i++) {
            fscanf(f, "%d", &need[i]);
            area += need[i] * shapes[i].area;
            for (int k = 0; k < need[i]; k++)
                pieces[npieces++] = i;
        }

        if (area > W * H) continue;

        qsort(pieces, npieces, sizeof(int), cmp_area);
        memset(board, 0, sizeof(board));

        if (solve(0)) count++;
    }

    clock_t end = clock();
    double t = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Number of regions that can fit all presents: %d\n", count);
    printf("Execution time: %.3f seconds\n", t);

    fclose(f);
    return 0;
}
