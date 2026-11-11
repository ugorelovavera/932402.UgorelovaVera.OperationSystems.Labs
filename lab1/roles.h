#ifndef ROLES_H
#define ROLES_H

#include <pthread.h>
#include "monitor.h"

/* Несериализуемые данные события: монитор передаёт указатель,
   не копируя и не сериализуя содержимое. */
typedef struct {
    int  id;
    char name[32];
} Payload;

typedef struct { Monitor *m; int count; } ProducerArgs;
typedef struct { Monitor *m; int count; } ConsumerArgs;

int producer_start(pthread_t *t, ProducerArgs *a);
int consumer_start(pthread_t *t, ConsumerArgs *a);

#endif
