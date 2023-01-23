//
// Created by Влад Кирилов on 29.11.2022.
//

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

string create_put_string(string message);
int create_get_string(string message, string buf, size_t size);

void init(queue_t *queue) {
    queue->data = (string*)malloc(MAX_SIZE * sizeof(char*));
    if (!queue->data) return;
    queue->size = 0;

    if (pthread_mutex_init(&queue->mutex, NULL)) {
        free(queue->data);
        queue->data = NULL;
        return;
    }

    if (pthread_cond_init(&queue->consumer_cond, NULL)) {
        pthread_mutex_destroy(&queue->mutex);
        free(queue->data);
        queue->data = NULL;
        return;
    }

    if (pthread_cond_init(&queue->producer_cond, NULL)) {
        pthread_mutex_destroy(&queue->mutex);
        pthread_cond_destroy(&queue->consumer_cond);
        free(queue->data);
        queue->data = NULL;
        return;
    }
}

void destroy(queue_t *queue) {
    free(queue->data);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->consumer_cond);
    pthread_cond_destroy(&queue->producer_cond);
}

int put(queue_t *queue, string message) {
    string str = create_put_string(message);

    if (pthread_mutex_lock(&queue->mutex)) {
        return -1;
    }

    while (queue->size == MAX_SIZE) {
        if (pthread_cond_wait(&queue->producer_cond, &queue->mutex)) {
            return -1;
        }
    }
    queue->data[queue->size] = str;
    queue->size++;

    pthread_cond_signal(&queue->consumer_cond);
    pthread_mutex_unlock(&queue->mutex);

    return (int)strlen(message);
}

int get(queue_t *queue, char *buf, size_t size) {
    if (pthread_mutex_lock(&queue->mutex)) {
        return -1;
    }

    while (queue->size == 0) {
        if (pthread_cond_wait(&queue->consumer_cond, &queue->mutex)) {
            return -1;
        }

    }
    string message = queue->data[0];

    for (int i = 0; i < queue->size - 1; ++i) {
        queue->data[i] = queue->data[i + 1];
    }

    queue->size--;
    if (pthread_cond_signal(&queue->producer_cond)) {
        return -1;
    }

    if (pthread_mutex_unlock(&queue->mutex)) {
        return -1;
    }

    int ret_size = create_get_string(message, buf, size);
    free(message);
    return ret_size;
}

string create_put_string(string message) { // срыв буфера
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

void drop(queue_t *queue) {
    if (pthread_mutex_lock(&queue->mutex)) {
        return;
    }

    if (queue->size == MAX_SIZE) {
        pthread_cond_broadcast(&queue->producer_cond);
    } else if (queue->size == 0) {
        pthread_cond_broadcast(&queue->consumer_cond);
    }

    if (pthread_mutex_unlock(&queue->mutex)) {
        return;
    }

}

int create_get_string(string message, string buf, size_t size) { // потенциальная возможность для утечки данных
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