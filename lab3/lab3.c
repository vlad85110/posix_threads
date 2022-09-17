#include "lab3.h"

#define Args struct args

struct args {
    char** strings;
    int len;
};

void print_strings(Args* args);

char** create_strings(int len);

void free_strings(Args **args, int len);

void lab3() {
    pthread_t childs[4];
    Args** arr = (Args**)malloc(4 * sizeof(Args*));

    for (int i = 0; i < 4; ++i) {
        pthread_t child;
        Args *args = (Args*) malloc(sizeof(Args));
        arr[i] = args;
        args->strings = create_strings(3);
        args->len = 3;

        if (pthread_create(&child, NULL, (void *) print_strings, (void *) args) != 0) {
            perror("thread_create");
        }

        childs[i] = child;
    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(childs[i], NULL);
    }

    free_strings(arr, 4);
}

char** create_strings(int len) {
    char** strings = (char **)malloc(3 * sizeof(char *));
    if (!strings) {
        return NULL;
    }

    static int sym = 'a';

    for (int i = 0; i < len; ++i) {
        char* str = (char *)malloc(5 * sizeof(char));

        if (!str) {
            return NULL;
        }

        for (int j = 0; j < 5; ++j) {
            str[j] = (char)(sym + j);
        }

        sym += 1;
        strings[i] = str;
    }

    return strings;
}

void print_strings(Args* args) {
    char **strings = args->strings;
    int len = args->len;

    for (int i = 0; i < len; ++i) {
        printf("%s\n", strings[i]);
    }
}

void free_strings(Args **args, int len) {
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < args[i]->len; ++j) {
            free(args[i]->strings[j]);
        }
        free(args[i]);
    }
}

