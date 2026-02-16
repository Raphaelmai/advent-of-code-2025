#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
    clock_t debut, fin;
    double temps;
    debut = clock();

    FILE *f = fopen("input.txt", "r");
    if (!f) return 1;

    int total = 0;
    int counter = -1;
    long long int range[1500][2] = {0};
    long long int values[1500] = {0};
    char buffer[250];
    int reading_ranges = 1;

    // Lecture du fichier
    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (reading_ranges && buffer[0] == '\0') {
            reading_ranges = 0;
            continue;
        }
        if (reading_ranges) {
            long long int a, b;
            if (sscanf(buffer, "%lld-%lld", &a, &b) == 2) {
                counter++;
                range[counter][0] = a;
                range[counter][1] = b;
            }
        }
        else {
            long long int id = atoll(buffer);
            int is_fresh = 0;
            for (int i = 0; i <= counter; i++) {
                if (range[i][0] <= id && id <= range[i][1]) {
                    is_fresh = 1;
                    break;
                }
            }
            if (is_fresh) {
                int already_used = 0;
                for (int k = 0; k < total; k++) {
                    if (values[k] == id) {
                        already_used = 1;
                        break;
                    }
                }
                if (!already_used) {
                    values[total++] = id;
                }
            }
        }
    }
    
    // PART 2 
    for (int i = 0; i <= counter; i++) {
        for (int j = i + 1; j <= counter; j++) {
            if (range[j][0] < range[i][0]) {
                long long int temp0 = range[i][0];
                long long int temp1 = range[i][1];
                range[i][0] = range[j][0];
                range[i][1] = range[j][1];
                range[j][0] = temp0;
                range[j][1] = temp1;
            }
        }
    }

    long long int merged[1500][2];
    int merged_count = 0;
    
    merged[0][0] = range[0][0];
    merged[0][1] = range[0][1];

    for (int i = 1; i <= counter; i++) {
        if (range[i][0] <= merged[merged_count][1] + 1) {
            if (range[i][1] > merged[merged_count][1]) {
                merged[merged_count][1] = range[i][1];
            }
        } else {
            merged_count++;
            merged[merged_count][0] = range[i][0];
            merged[merged_count][1] = range[i][1];
        }
    }

    long long int total_fresh = 0;
    for (int i = 0; i <= merged_count; i++) {
        total_fresh += (merged[i][1] - merged[i][0] + 1);
    }

    fin = clock();
    temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fclose(f);

    printf("There are %d fresh ingredients\n", total);
    printf("There are %lld freshed ingredients.. What a score\n", total_fresh);
    printf("Temps d'exécution : %.3f secondes\n", temps);

    return 0;
}
