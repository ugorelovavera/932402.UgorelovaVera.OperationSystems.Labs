#include "roles.h"
#include <assert.h>
#include <stdio.h>

static void test_roundtrip(void) {
    Monitor m;
    assert(monitor_init(&m) == 0);
    Event in = { .payload = (void *)42L, .lifetime = 7 };
    assert(monitor_provide(&m, &in) == 0);
    Event out;
    assert(monitor_consume(&m, &out) == 0);
    assert(out.payload == in.payload);
    assert(out.lifetime == in.lifetime);
    monitor_destroy(&m);
}
static void test_stress(void) {
    Monitor m;
    assert(monitor_init(&m) == 0);
    ProducerArgs pa = { .m = &m, .count = 1000 };
    ConsumerArgs ca = { .m = &m, .count = 1000 };
    pthread_t pt, ct;
    assert(producer_start(&pt, &pa) == 0);
    assert(consumer_start(&ct, &ca) == 0);
    pthread_join(pt, NULL);
    monitor_stop(&m);
    pthread_join(ct, NULL);
    monitor_destroy(&m);
}

int main(void) {
    test_roundtrip();
    test_stress();
    puts("all tests passed");
    return 0;
}
