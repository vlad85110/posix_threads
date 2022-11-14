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

    return node;
}

_list* create_list() {
    _list *list = (_list*)malloc(sizeof(_list));

    if (!list) {
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

    list->size++;
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

    _node *it = list->first->next;

    while (it != NULL) {
        pthread_mutex_lock(&it->mutex);

        if (it->next != NULL) {
            fprintf(stderr, "%s -> ", it->string);
        } else {
            fprintf(stderr, "%s", it->string);
        }

        pthread_mutex_unlock(&it->mutex);
        it = it->next;
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

    return true;
}

_node* get_elem(_list* list,int index) {
    _node* elem = list->first;

    for (int i = 0; i < index; ++i) {
        pthread_mutex_t mutex = elem->mutex;
        pthread_mutex_lock(&mutex);
        elem = elem->next;
        pthread_mutex_unlock(&mutex);
    }

    return elem;
}

void sort(_list* list) {
    _node* i_el, *j_el;

    if (list->size <= 1) {
        return;
    }

    i_el = get_elem(list, 0);
    for (int i = 0; i < list->size; ++i) {
        j_el = get_elem(list, i + 1);
        for (int j = i + 1; j < list->size; ++j) {
            pthread_mutex_t i_mutex = i_el->mutex, j_mutex = j_el->mutex;
            pthread_mutex_lock(&i_mutex);
            pthread_mutex_lock(&j_mutex);

            if (compare(i_el->next, j_el->next)) {
                swap(i_el, j_el);
                j_el = i_el->next;
            }

            pthread_mutex_unlock(&j_mutex);
            pthread_mutex_unlock(&i_mutex);

            j_el = j_el->next;
        }
        i_el = i_el->next;
    }
}
