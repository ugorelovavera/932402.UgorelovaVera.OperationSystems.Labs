#include "roles.h"
static void *producer_run(void *arg) {
    ProducerArgs *a = arg;
    for (int i = 0; i < a->count; ++i) {
        Event e = { .payload = (void *)(long)(i + 1),
                    .lifetime = (size_t)(i + 1) };
        if (monitor_provide(a->m, &e) != 0) break; 
    }
    return NULL;
}
int producer_start(pthread_t *t, ProducerArgs *a) {
    return pthread_create(t, NULL, producer_run, a);
}