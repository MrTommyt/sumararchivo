// Camilo Alberto Benavides Jimenez (2021114019)
// http://github.com/MrTommyt/sumararchivo
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

#define START_TIMER(id) double id##_elapsed; \
    struct timespec id##_start, id##_end; \
    clock_gettime(CLOCK_REALTIME, &id##_start);

#define STOP_TIMER(id) clock_gettime(CLOCK_REALTIME, &id##_end); \
    id##_elapsed = interval_in_ms(id##_start, id##_end);

const double ns_in_ms = 1000000;

#define MAX_VALUE 10000

double interval_in_ms(struct timespec start, struct timespec end);

int main() {
    if (stdin == NULL) {
        perror("Entrada no encontrada");
        exit(EXIT_FAILURE);
    }

    if (isatty(0)) {
        perror("La entrada tiene que ser dada mediante el STDIN: (ej: ./main < test1.in)");
        exit(EXIT_FAILURE);
    }

    int size = 0;
    fscanf(stdin, "%d\n", &size); // NOLINT(*-err34-c)
    printf("size: %d\n", size);

    int *freq_values = calloc(MAX_VALUE, sizeof(freq_values));

    int *arr = (int *) malloc(size * sizeof(int));
    long sum = 0;
    START_TIMER(total)

    START_TIMER(sum_time);
    for (int i = 0; i < size; i++) {
        fscanf(stdin, "%d", &arr[i]); // NOLINT(*-err34-c)
        sum += arr[i];
        freq_values[arr[i]]++;
    }
    STOP_TIMER(sum_time)

    double avg = (double) sum / size;
    double sum_var = 0;

    int is_even_size = size % 2 == 0;
    int size_half = size / 2;

    bool med_found = 0;
    int med1 = -1, med2 = -1;
    int idx = 0;

    int most_common = -1, most_common_reps = -1;
    int most_common_1 = -1, most_common_reps_1 = -1;

    START_TIMER(extra)
    for (int num = 0; num < MAX_VALUE; num++) {
        const int freq = freq_values[num];
        if (freq == 0) {
            continue;
        }

        if (freq >= most_common_reps) {
            most_common_1 = most_common;
            most_common_reps_1 = most_common_reps;

            most_common = num;
            most_common_reps = freq;
        }

        int new_idx = idx + freq;
        int new_idx_diff = new_idx - size_half;
        if (!med_found) {
            if (new_idx_diff > 0) {
                if (med1 == -1) {
                    med1 = num;
                }
                if (is_even_size) {
                    if (new_idx_diff > 1) {
                        med2 = num;
                        med_found = true;
                    }
                } else {
                    med_found = true;
                }
            }
        }

        sum_var += pow((num - avg), 2) * freq;
        idx = new_idx;
    }

    double var = sum_var / size;
    double std_deviation = sqrt(var);
    STOP_TIMER(extra)

    STOP_TIMER(total)
    printf("The sum of all the file is: %ld\n", sum);
    printf("sum_time elapsed: %fms\n", sum_time_elapsed);
    printf("median: %d\n", is_even_size ? ((med1 + med2) / 2) : med1);
    printf("variance: %f\n", var);
    printf("standard deviation: %f\n", std_deviation);
    printf("extra elapsed: %fms\n", extra_elapsed);
    printf("total time elapsed: %fms\n", total_elapsed);
    printf("most common (value, reps): (%d, %d); (%d, %d)\n",
        most_common, most_common_reps, most_common_1, most_common_reps_1);

    free(arr);
    free(freq_values);
}

double interval_in_ms(struct timespec start, struct timespec end) {
    return ((double) ((end.tv_sec - start.tv_sec) * 1000))
        + ((double) (end.tv_nsec - start.tv_nsec) / ns_in_ms);
}
