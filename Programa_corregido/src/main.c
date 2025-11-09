#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "buffer.h"
#include "crc.h"
#include "config.h"
#include <stdatomic.h>

static _Atomic int running = 1;

void* producer(void* arg) {
    if (arg == NULL) {
       return;
    }

    RingBuffer* rb = (RingBuffer*)arg;
    for (int i = 0; i < 1000; ++i) {
        while (rb_push(rb, (uint8_t)(i & 0xFF)) != 0) {
            usleep(100);
        }
    }

    atomic_store(&running, 0);

    return NULL;
}

void* consumer(void* arg) {
    if (arg == NULL) {
       return;
    }
    RingBuffer* rb = (RingBuffer*)arg;
    uint8_t v;
    while (atomic_load(&running) || rb_count(rb) > 0) {
        if (rb_pop(rb, &v) == 0){
            char msg[8];
            int result = snprintf(msg, sizeof(msg), "v=%d", v);
            if (result != strlen(msg)){
                return;
            }
            (void)msg;
        }else{
            usleep(100);
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <config>\n", argv[0]);
    }

    AppConfig cfg;
    load_config((argc > 1) ? argv[1] : "tests/example.cfg", &cfg);

    RingBuffer rb;
    rb_init(&rb, 16);

    pthread_t th_prod, th_cons;
    
    pthread_create(&th_cons, NULL, consumer, &rb);
    pthread_create(&th_prod, NULL, producer, &rb);
    
    char data[32] = "hello";
    uint32_t c = crc32_compute(data, strlen(data));
    printf("CRC=%08x\n", c);

    pthread_join(th_prod, NULL);
    pthread_join(th_cons, NULL);

    rb_free(&rb);
    rb_free(&rb);

    return 0;
}
