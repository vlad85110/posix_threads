//
// Created by Влад Кирилов on 23.10.2022.
//

#include "list.h"

#include <string.h>

_node* create_node(const String string) {
    _node *node = (_node*)malloc(sizeof(_node));
    if (!node) {
        return NULL;
    }

    if (string) {
        node->string = (String) malloc((strlen(string) + 1) * sizeof(char));
        if (!node->string) {
            free(node);
            return NULL;
        }
        strcpy(node->string, string);
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

    node->next = list->first->next;
    list->first->next = node;

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
        if (it->next != NULL) {
            fprintf(stderr, "%s -> ", it->string);
        } else {
            fprintf(stderr, "%s", it->string);
        }
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
        elem = elem->next;
    }

    return elem;
}

void sort(_list* list) {
    _node* i_el, *j_el;

    for (int i = 0; i < list->size; ++i) {
        for (int j = i + 1; j < list->size; ++j) {
            i_el = get_elem(list, i);
            j_el = get_elem(list, j);

            if (compare(i_el->next, j_el->next)) {
                swap(i_el, j_el);
            }
        }
    }
}
