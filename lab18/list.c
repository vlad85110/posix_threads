//
// Created by Влад Кирилов on 23.10.2022.
//

#include "list.h"

#include <string.h>
#include <pthread.h>

_node* create_node(const String string) {
    _node *node = (_node*)malloc(sizeof(_node));
    if (!node) {
        return NULL;
    }

    if (string) {
        node->string = (String)malloc((strlen(string) + 1) * sizeof(char));
        if (!node->string) {
            free(node);
            return NULL;
        }

        strcpy(node->string, string);
    }

    if (pthread_mutex_init(&node->mutex, NULL)) {
        free(node->string);
        free(node);
        return NULL;
    }
    node->addr = &node->mutex;
    node->next = NULL;

    return node;
}

_list* create_list() {
    _list *list = (_list*)malloc(sizeof(_list));

    if (!list) {
        return NULL;
    }

    if (pthread_mutex_init(&list->mutex, NULL)) {
        free(list);
        return NULL;
    }

    list->first = create_node(NULL);
    list->size = 0;

    return list;
}

bool add_to_begin(_list *list, const String string) {
    _node *node = create_node(string);
    if (!node) {
        return false;
    }

    pthread_mutex_lock(&list->first->mutex);
    node->next = list->first->next;
    list->first->next = node;
    pthread_mutex_unlock(&list->first->mutex);

    pthread_mutex_lock(&list->mutex);
    list->size++;
    pthread_mutex_unlock(&list->mutex);
    return true;
}

bool is_empty(const _list *list) {
    return list->size == 0;
}

void delete_list(_list *list) {
    _node *it = list->first, *prev;

    while (it->next != NULL) {
        prev = it;
        it = it->next;
        free(prev->string);
        pthread_mutex_destroy(&prev->mutex);
        free(prev);
    }

    free(it);
    free(list);
}

void print_list(const _list *list) {
    if (is_empty(list)) {
        fprintf(stderr, "empty\n");
        return;
    }

    _node *it = list->first, *prev;
    pthread_mutex_lock(&it->mutex);

    while (it != NULL) {
        if (it->next != NULL) {
            if (it->string) {
                fprintf(stderr, "%s -> ", it->string);
            }
        } else {
            if (it->string) {
                fprintf(stderr, "%s", it->string);
            }
        }

        prev = it;
        it = it->next;

        if (it != NULL) {
            pthread_mutex_lock(&it->mutex);
        }

        pthread_mutex_unlock(&prev->mutex);
    }

    fprintf(stderr, "\n");
}

void swap(_node* first, _node* second) {
    _node* tmp, *elem1 = first->next, *elem2 = second->next;

    first->next = elem2;
    second->next = elem1;
    tmp = elem1->next;
    elem1->next = elem2->next;
    elem2->next = tmp;
}

bool compare(_node* first, _node* second) {
    size_t len;
    size_t first_len = strlen(first->string);
    size_t second_len = strlen(second->string);

    if (first_len >= second_len) {
        len = second_len;
    } else {
        len = first_len;
    }

    for (size_t i = 0; i < len; ++i) {
        if (first->string[i] < second->string[i]) {
            return false;
        }

        if (first->string[i] > second->string[i]) {
            return true;
        }
    }

    if (first_len < second_len) {
        return false;
    } else if (second_len < first_len) {
        return true;
    }

    return false;
}

void sort(_list* list) {
    pthread_mutex_lock(&list->mutex);
    size_t size = list->size;
    pthread_mutex_unlock(&list->mutex);

    if (size <= 1) {
        return;
    }

    char tr[size + 1];
    tr[size] = '\0';
    for (int i = 0; i < size + 1; ++i) {
        tr[i] = '-';
    }

    _node* i_el, *j_el, *i_prev,  *j_prev;
    bool is_swap;

    i_el = list->first;
    pthread_mutex_lock(&i_el->mutex);
//    tr[0] = 'x';fprintf(stderr, "%s l %p\n", tr, &i_el->mutex);

    for (int i = 0; i < size; ++i) {
        j_el = i_el->next;

        pthread_mutex_lock(&j_el->mutex);
//        tr[i + 1] = 'x';fprintf(stderr, "%s l %p\n", tr, &j_el->mutex);

        for (int j = i + 1; j < size; ++j) {
            pthread_mutex_lock(&j_el->next->mutex);
            if (i_el->next != j_el) {
                pthread_mutex_lock(&i_el->next->mutex);
            }

//            tr[j + 1] = 'x';fprintf(stderr, "%s l %p\n", tr, &j_el->next->mutex);

            if (compare(i_el->next, j_el->next)) {
                is_swap = i_el->next == j_el;

                swap(i_el, j_el);
//                fprintf(stderr, "swap\n");

                if (is_swap) {
                    j_el = i_el->next;
                }
            }

            j_prev = j_el;
            j_el = j_el->next;

            if (i_el->next != j_el) {
                pthread_mutex_unlock(&i_el->next->mutex);
            }
            pthread_mutex_unlock(&j_prev->mutex);
//            tr[j] = '-';fprintf(stderr, "%s u %p\n", tr, &j_prev->mutex);
        }

        pthread_mutex_unlock(&j_el->mutex);
//        tr[size] = '-';fprintf(stderr, "%s u %p\n", tr, &j_el->mutex);

//        fprintf(stderr, "%p\n", &i_el->next->mutex);
        pthread_mutex_lock(&i_el->next->mutex);
//        tr[i + 1] = 'x';fprintf(stderr, "%s\n", tr);

        i_prev = i_el;
        i_el = i_el->next;
        pthread_mutex_unlock(&i_prev->mutex);
//        tr[i] = '-';fprintf(stderr, "%s u %p\n", tr, &i_prev->mutex);
    }

    pthread_mutex_unlock(&i_el->mutex);
//    tr[size] = '-';fprintf(stderr, "%s\n", tr);
}
