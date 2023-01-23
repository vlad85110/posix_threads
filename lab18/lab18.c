//
// Created by Влад Кирилов on 23.10.2022.
//

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lab18.h"
#include "list.h"


_Noreturn void child_fun(_list* list);

void lab18() {
    _list* list = create_list();
    if (!list) return;

    pthread_t child;

    for (int i = 0; i < 10; ++i) {
        if (pthread_create(&child, NULL, (void *)child_fun, list)) {
            perror("thread_create");
            return;
        }
    }

    char input[1000 + 1];
    while (true) {
        fgets(input, 1000, stdin);
        input[1000] = '\0';

        if (strlen(input) == 1 && input[0] == '\n') {
            print_list(list);
            continue;
        }

        if (strlen(input) == 2 && input[0] == 'q') {
            break;
        }

        if (strlen(input) >= 80) {
            for (int i = 0; i < strlen(input); i += 40) {
                char str[41];
                size_t len, str_len = strlen(input + i);
                if (str_len < 40) {
                    len = str_len;
                } else {
                    len = 40;
                }

                strncpy(str, input + i, len);
                str[len - 1] = '\0';
                add_to_begin(list, str);
            }
        } else {
            input[strlen(input) - 1] = '\0';
            add_to_begin(list, input);
        }
    }

    delete_list(list);
}

_Noreturn void child_fun(_list* list) {
    while (true) {
        //sleep(1);
        sort(list);
    }
}