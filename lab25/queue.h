//
// Created by Влад Кирилов on 29.11.2022.
//

#include <sys/types.h>
#include <semaphore.h>

#define MAX_SIZE 10

typedef struct queue queue_t;
typedef char* string;

struct queue {
    string* data;
    size_t size;

    sem_t mutex;
    sem_t full;
    sem_t empty;
};

void init(queue_t *);
void drop(queue_t *);
void destroy(queue_t *);

int put(queue_t *, char *);
int get(queue_t *, char *buf, size_t size);

void print(queue_t *);