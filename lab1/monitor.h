#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
typedef struct {
    void *payload;
    size_t lifetime;
} Event;
typedef struct {
    pthread_mutex_t mutex;      
    pthread_cond_t condition;  
    bool ready;     
    bool stopping;   
    Event data;      
} Monitor;
int  monitor_init(Monitor *m);
void monitor_destroy(Monitor *m);
int  monitor_provide(Monitor *m, const Event *e); 
int  monitor_consume(Monitor *m, Event *out);      
int  monitor_stop(Monitor *m);                    
#endif