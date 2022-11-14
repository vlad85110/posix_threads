//
// Created by Влад Кирилов on 23.10.2022.
//

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#define _node struct node
#define _list struct list
#define String char*

_node {
    String string;
    _node* next;
    pthread_mutex_t mutex;
};

_list {
    _node *first;
    int size;
};

_list* create_list();
bool add_to_begin(_list *list, const String string);
bool is_empty(const _list *list);
void delete_list(_list *list);
void print_list(const _list *list);
void sort(_list* list);
