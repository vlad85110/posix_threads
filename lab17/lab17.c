//
// Created by Влад Кирилов on 23.10.2022.
//

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lab17.h"
#include "list.h"


_Noreturn void child_fun(_list* list);

pthread_mutex_t mutex;

void lab17() {
    _list* list = create_list();
    if (!list) return;

    pthread_t child;

    if (pthread_mutex_init(&mutex, NULL)) {
        perror("mutex_init");
        return;
    }

    if (pthread_create(&child, NULL, (void *)child_fun, list)) {
        perror("thread_create");
        return;
    }

    char input[1000 + 1];
    while (true) {
        fgets(input, 1000, stdin);
        input[1000] = '\0';

        if (strlen(input) == 0) {
            pthread_mutex_lock(&mutex);
            print_list(list);
            pthread_mutex_unlock(&mutex);
            continue;
        }

        if (strlen(input) >= 80) {
            for (int i = 0; i < strlen(input); i += 40) {
                pthread_mutex_lock(&mutex);

                char str[41];
                size_t len, str_len = strlen(input + i);
                if (str_len < 40) {
                    len = str_len;
                } else {
                    len = 40;
                }

                strncpy(str, input + i, len);
                str[len] = '\0';
                add_to_begin(list, str);

                pthread_mutex_unlock(&mutex);
            }
        } else {
            pthread_mutex_lock(&mutex);
            add_to_begin(list, input);
            pthread_mutex_unlock(&mutex);
        }
    }
}

_Noreturn void child_fun(_list* list) {
    while (true) {
        sleep(5);
        pthread_mutex_lock(&mutex);
        sort(list);
        pthread_mutex_unlock(&mutex);
    }
}