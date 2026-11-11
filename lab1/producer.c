#include "roles.h"
#include <stdio.h>
#include <unistd.h>

#define MAX_EVENTS 10

static Payload payloads[MAX_EVENTS];

static void *producer_run(void *arg) {
    ProducerArgs *a = arg;
    for (int i = 0; i < a->count && i < MAX_EVENTS; ++i) {
        sleep(1);

        payloads[i].id = i + 1;
        snprintf(payloads[i].name, sizeof(payloads[i].name),
                 "event-%d", i + 1);

        Event e = { .payload  = &payloads[i],
                    .lifetime = (size_t)(i + 1) };

        printf("produce: id=%d name=%s\n",
               payloads[i].id, payloads[i].name);
        fflush(stdout);

        if (monitor_provide(a->m, &e) != 0) break;
    }
    return NULL;
}

int producer_start(pthread_t *t, ProducerArgs *a) {
    return pthread_create(t, NULL, producer_run, a);
}
