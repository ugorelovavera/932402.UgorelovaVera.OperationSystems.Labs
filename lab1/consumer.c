#include "roles.h"
#include <stdio.h>

static void *consumer_run(void *arg) {
    ConsumerArgs *a = arg;
    int got = 0;
    for (;;) {
        Event e;
        int rc = monitor_consume(a->m, &e);
        if (rc != 0) break;

        Payload *p = e.payload;
        printf("consume: id=%d name=%s\n", p->id, p->name);
        fflush(stdout);

        if (++got >= a->count) break;
    }
    return NULL;
}

int consumer_start(pthread_t *t, ConsumerArgs *a) {
    return pthread_create(t, NULL, consumer_run, a);
}
