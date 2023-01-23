//
// Created by Влад Кирилов on 29.11.2022.
//

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

string create_put_string(string message);
int create_get_string(string message, string buf, size_t size);

void init(queue_t *queue) {
    queue->data = (string*)malloc(MAX_SIZE * sizeof(char*));
    if (!queue->data) return;
    queue->size = 0;

    if ((sem_init(&queue->mutex, 0, 1) < 0) || (sem_init(&queue->full, 0, 0) < 0) ||
            (sem_init(&queue->empty, 0, MAX_SIZE) < 0)) {
        free(queue->data);
        return;
    }
}

void destroy(queue_t *queue) {
    free(queue->data);

    sem_destroy(&queue->mutex);
    sem_destroy(&queue->full);
    sem_destroy(&queue->empty);
}

int put(queue_t *queue, string message) {
    string str = create_put_string(message);

    sem_wait(&queue->empty);
    sem_wait(&queue->mutex);

    queue->data[queue->size] = str;
    queue->size++;

    sem_post(&queue->mutex);
    sem_post(&queue->full);

    return (int)strlen(message);
}

int get(queue_t *queue, char *buf, size_t size) {
    sem_wait(&queue->full);
    sem_wait(&queue->mutex);
    string message = queue->data[0];

    for (int i = 0; i < queue->size - 1; ++i) {
        queue->data[i] = queue->data[i + 1];
    }

    queue->size--;
    sem_post(&queue->mutex);
    sem_post(&queue->empty);

    int ret_size = create_get_string(message, buf, size);
    free(message);
    return ret_size;
}

void drop(queue_t *queue) {

}

void print(queue_t *queue) {
    if (queue->size == 0) {
        fprintf(stderr, "empty\n");
        return;
    }

    for (int i = 0; i < queue->size; ++i) {
        fprintf(stderr, "%s ", queue->data[i]);
    }
    fprintf(stderr, "\n");
}

string create_put_string(string message) {
    size_t str_len, message_len = strlen(message);
    if (message_len <= 80) {
        str_len = message_len;
    } else {
        str_len = 80;
    }

    string str = (string)malloc((str_len + 1) * sizeof(char));
    if (!str) {
        return str;
    }

    strcpy(str, message);
    return str;
}

int create_get_string(string message, string buf, size_t size) {
    size_t message_len = strlen(message);
    size_t ret_size;
    if (message_len + 1 > size) {
        ret_size = message_len;
    } else {
        ret_size = size - 1;
    }

    strncpy(buf, message, size - 1);
    return (int)ret_size;
}


//именованный семафор