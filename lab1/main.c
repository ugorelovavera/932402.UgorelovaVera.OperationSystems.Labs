#include "roles.h"
#include <stdio.h>

int main(void) {
    Monitor m;
    if (monitor_init(&m) != 0) { perror("monitor_init"); return 1; }

    ProducerArgs pa = { .m = &m, .count = 5 };
    ConsumerArgs ca = { .m = &m, .count = 5 };

    pthread_t pt, ct;
    if (producer_start(&pt, &pa) != 0) { perror("producer_start"); return 1; }
    if (consumer_start(&ct, &ca) != 0) { perror("consumer_start"); return 1; }

    pthread_join(pt, NULL);
    monitor_stop(&m);
    pthread_join(ct, NULL);

    monitor_destroy(&m);
    return 0;
}
