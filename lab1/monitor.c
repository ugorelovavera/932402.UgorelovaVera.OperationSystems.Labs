#include "monitor.h"
#include <errno.h>

int monitor_init(Monitor *m) {
    int rc = pthread_mutex_init(&m->mutex, NULL);
    if (rc) return rc;
    rc = pthread_cond_init(&m->condition, NULL);
    if (rc) {
        pthread_mutex_destroy(&m->mutex);   /* откат: мьютекс уже создан */
        return rc;
    }
    m->ready = false;
    m->stopping = false;
    m->data.payload = NULL;
    m->data.lifetime = 0;
    return 0;}
void monitor_destroy(Monitor *m) {
    pthread_cond_destroy(&m->condition);
    pthread_mutex_destroy(&m->mutex);
}
int monitor_provide(Monitor *m, const Event *e) {
    pthread_mutex_lock(&m->mutex);          
    while (m->ready && !m->stopping) {
        pthread_mutex_unlock(&m->mutex);
        struct timespec ts = {0, 1000000};     
        nanosleep(&ts, NULL);
        pthread_mutex_lock(&m->mutex);
    }

    if (m->stopping) {
        pthread_mutex_unlock(&m->mutex);
        return ECANCELED;
    }
    m->data  = *e;                              /* 3. записать данные */
    m->ready = true;                            /* 4. ready = 1 */
    pthread_cond_signal(&m->condition);         /* 5. уведомить одного */
    pthread_mutex_unlock(&m->mutex);            /* 6. освободить */
    return 0;
}
int monitor_consume(Monitor *m, Event *out) {
    pthread_mutex_lock(&m->mutex);
    while (!m->ready && !m->stopping) {
        pthread_cond_wait(&m->condition, &m->mutex);
    }
    if (m->stopping && !m->ready) {            
        pthread_mutex_unlock(&m->mutex);
        return ECANCELED;
    }
    *out = m->data;                             
    m->ready = false;                           
    pthread_mutex_unlock(&m->mutex);
    return 0;
}
int monitor_stop(Monitor *m) {
    pthread_mutex_lock(&m->mutex);              
    m->stopping = true;                       
    pthread_cond_broadcast(&m->condition);      
    pthread_mutex_unlock(&m->mutex);           
}