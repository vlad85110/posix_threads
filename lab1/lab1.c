#include "lab1.h"

void print_text();

void lab1() {
    pthread_t child;

    if (pthread_create(&child, NULL, (void *)print_text, NULL)) {
        perror("thread_create");
    }

    pthread_join(child, NULL);
    print_text();
}

void print_text() {
    for (int i = 0; i < 10; ++i) {
        printf("%d \n", i);
    }
}
