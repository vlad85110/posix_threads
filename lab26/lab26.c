//
// Created by Влад Кирилов on 08.12.2022.
//

#include "lab26.h"
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

_Noreturn void consumer(queue_t* queue);

_Noreturn void producer(queue_t* queue);

void lab26() {
    queue_t queue;
    init(&queue);

    pthread_t consumer1, consumer2, producer1, producer2;

    if (pthread_create(&consumer1, NULL, (void *)consumer, &queue)) {
        perror("thread_create");
        return;
    }

    if (pthread_create(&consumer2, NULL, (void *)consumer, &queue)) {
        perror("thread_create");
        return;
    }

    if (pthread_create(&producer1, NULL, (void *)producer, &queue)) {
        perror("thread_create");
        return;
    }

//    if (pthread_create(&producer2, NULL, (void *)producer, &queue)) {
//        perror("thread_create");
//        return;
//    }

    pthread_join(consumer1, NULL);
}

_Noreturn void producer(queue_t* queue) {
//    string str = "123";
//    while (1) {
//        put(queue, str);
//        fprintf(stderr, "producer\n");
//        sleep(1);
//    }

    sleep(1);
    drop(queue);
}

_Noreturn void consumer(queue_t* queue) {
    char str[30];

    while(1) {
        get(queue, str, 30);
        fprintf(stderr, "consumer \n");
        sleep(1);
    }
}