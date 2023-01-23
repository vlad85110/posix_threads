//
// Created by Влад Кирилов on 20.01.2023.
//

#include "lab8.h"

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

#define ITER_COUNT 30000l//30000000000

struct args {
    double thread_sum;
    long iter_count;
    long start_index;
};

typedef struct args args_t;

void pi_count(args_t *args) {
    double denominator;

    for (long i = args->start_index; i < args->iter_count; ++i) {
        denominator = 2.0 * (double)i + 1;
        args->thread_sum += pow(-1, (double)i) / denominator;
    }
}

void lab8(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr, "no thread count passed\n");
        return;
    }

    long thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0) {
        fprintf(stderr, "incorrect thread count\n");
        return;
    }

    double pi4 = 0;
    pthread_t threads[thread_count - 1];
    args_t thread_args[thread_count];
    long iter_per_thread = ITER_COUNT / thread_count;
    printf("%ld - %ld\n", ITER_COUNT, thread_count * iter_per_thread);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for (long i = 0; i < thread_count; ++i) {
        thread_args[i].thread_sum = 0;
        thread_args[i].iter_count = iter_per_thread;
        thread_args[i].start_index = i * iter_per_thread;

        if (i != thread_count - 1) {
            int res = pthread_create(&threads[i], NULL, (void *)pi_count, (void *)&thread_args[i]);

            if (res != 0) {
                perror("thread_create");
                exit(1);
            }
        }
    }

    pi_count(&thread_args[thread_count - 1]);

    for (long i = 0; i < thread_count; ++i) {
        if (i != thread_count - 1) {
            if (pthread_join(threads[i], NULL)) {
                perror("pthread_join");
            }
        }

        pi4 += thread_args[i].thread_sum;
    }

    clock_gettime(CLOCK_REALTIME, &end);
//    printf("%lf\n",
//           (double)(end.tv_sec-start.tv_sec)
//           + 0.000000001*(double)(end.tv_nsec-start.tv_nsec));

    printf("%.10lf\n", pi4 * 4);
}
