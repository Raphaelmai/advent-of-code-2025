#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#define MAX_TILES 2000
#define EPSILON 0.000001

typedef struct {
    int x, y;
} Point;

int read_tiles(const char *filename, Point tiles[]) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Erreur: impossible d'ouvrir %s\n", filename);
        return -1;
    }
    
    int count = 0;
    while (fscanf(f, "%d,%d", &tiles[count].x, &tiles[count].y) == 2) {
        count++;
    }
    fclose(f);
    return count;
}

long long part1(Point tiles[], int n) {
    long long max_area = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long width = llabs((long long)tiles[j].x - tiles[i].x) + 1;
            long long height = llabs((long long)tiles[j].y - tiles[i].y) + 1;
            long long area = width * height;
            
            if (area > max_area) {
                max_area = area;
            }
        }
    }
    
    return max_area;
}

int point_in_polygon(int px, int py, Point tiles[], int n) {
    int crossings = 0;
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int x1 = tiles[i].x, y1 = tiles[i].y;
        int x2 = tiles[j].x, y2 = tiles[j].y;
        
        if ((y1 <= py && py < y2) || (y2 <= py && py < y1)) {
            double x_intersect = (double)x1 + (double)(py - y1) * (x2 - x1) / (y2 - y1);
            if (px < x_intersect) {
                crossings++;
            }
        }
    }
    
    return (crossings % 2 == 1);
}

int segment_crosses_segment(double p1x, double p1y, double p2x, double p2y,
                            double q1x, double q1y, double q2x, double q2y) {
    double d = (p2x - p1x) * (q2y - q1y) - (p2y - p1y) * (q2x - q1x);
    
    if (fabs(d) < EPSILON) return 0;
    
    double t = ((q1x - p1x) * (q2y - q1y) - (q1y - p1y) * (q2x - q1x)) / d;
    double u = ((q1x - p1x) * (p2y - p1y) - (q1y - p1y) * (p2x - p1x)) / d;
    
    if (t > EPSILON && t < 1.0 - EPSILON && u > EPSILON && u < 1.0 - EPSILON) {
        return 1;
    }
    
    return 0;
}

int polygon_cuts_rectangle(int min_x, int min_y, int max_x, int max_y, Point tiles[], int n) {
    double rect_edges[4][4] = {
        {(double)min_x, (double)min_y, (double)max_x, (double)min_y},
        {(double)max_x, (double)min_y, (double)max_x, (double)max_y},
        {(double)max_x, (double)max_y, (double)min_x, (double)max_y},
        {(double)min_x, (double)max_y, (double)min_x, (double)min_y}
    };
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double px1 = tiles[i].x, py1 = tiles[i].y;
        double px2 = tiles[j].x, py2 = tiles[j].y;
        
        for (int e = 0; e < 4; e++) {
            if (segment_crosses_segment(px1, py1, px2, py2,
                                       rect_edges[e][0], rect_edges[e][1],
                                       rect_edges[e][2], rect_edges[e][3])) {
                return 1;
            }
        }
    }
    
    return 0;
}

int point_on_segment(int px, int py, int x1, int y1, int x2, int y2) {
    long long cross = (long long)(py - y1) * (x2 - x1) - (long long)(y2 - y1) * (px - x1);
    if (cross != 0) return 0;
    
    if (px < (x1 < x2 ? x1 : x2) || px > (x1 > x2 ? x1 : x2)) return 0;
    if (py < (y1 < y2 ? y1 : y2) || py > (y1 > y2 ? y1 : y2)) return 0;
    
    return 1;
}

int point_is_green_or_red(int px, int py, Point tiles[], int n) {
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (point_on_segment(px, py, tiles[i].x, tiles[i].y, tiles[j].x, tiles[j].y)) {
            return 1;
        }
    }
    
    return point_in_polygon(px, py, tiles, n);
}

long long part2(Point tiles[], int n) {
    long long max_area = 0;
    
    for (int i = 0; i < n; i++) {

        
        for (int j = i + 1; j < n; j++) {
            int x1 = tiles[i].x, y1 = tiles[i].y;
            int x2 = tiles[j].x, y2 = tiles[j].y;
            
            int min_x = (x1 < x2) ? x1 : x2;
            int max_x = (x1 > x2) ? x1 : x2;
            int min_y = (y1 < y2) ? y1 : y2;
            int max_y = (y1 > y2) ? y1 : y2;
            
            long long width = max_x - min_x + 1;
            long long height = max_y - min_y + 1;
            long long area = width * height;
            
            if (area <= max_area) continue;
            
            if (polygon_cuts_rectangle(min_x, min_y, max_x, max_y, tiles, n)) {
                continue;
            }
            
            if (!point_is_green_or_red(min_x, min_y, tiles, n)) continue;
            if (!point_is_green_or_red(max_x, min_y, tiles, n)) continue;
            if (!point_is_green_or_red(max_x, max_y, tiles, n)) continue;
            if (!point_is_green_or_red(min_x, max_y, tiles, n)) continue;
            
            max_area = area;
        }
    }
    
    return max_area;
}

int main(int argc, char *argv[]) {
    clock_t debut, fin;
    double temps;
    debut = clock();
    const char *filename = (argc > 1) ? argv[1] : "input.txt";
    
    Point tiles[MAX_TILES];
    int n = read_tiles(filename, tiles);
    
    if (n < 0) return 1;
    
    
    long long result1 = part1(tiles, n);
    
    long long result2 = part2(tiles, n);
    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    printf("The largest area of any rectangle you can make is %lld\n", result1);
    printf("The largest area of any rectangle you can make using only red and green tiles is %lld\n", result2);
    printf("Temps d'exécution : %.3f secondes\n", temps);
    return 0;
}