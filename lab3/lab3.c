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
    pthread_t children[4];
    Args** arr = (Args**)malloc(4 * sizeof(Args*));

    for (int i = 0; i < 4; ++i) {
        pthread_t child;
        Args *args = (Args*)malloc(sizeof(Args));

        if (!args) {
            return;
        }

        arr[i] = args;
        args->strings = create_strings(3);
        args->len = 3;

        if (pthread_create(&child, NULL, (void *) print_strings, (void *) args) != 0) {
            perror("thread_create");
        }

        children[i] = child;
    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(children[i], NULL);
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
        char* str = (char *)malloc(6 * sizeof(char));

        if (!str) {
            return NULL;
        }

        for (int j = 0; j < 5; ++j) {
            str[j] = (char)(sym + j);
        }
        str[5] = '\0';

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

    pthread_exit(0);
}

void free_strings(Args **args, int len) {
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < args[i]->len; ++j) {
            free(args[i]->strings[j]);
        }
        free(args[i]);
    }
}


//что происходит при завершении

//что остается

//где хранится информация о звершившимся потоке



