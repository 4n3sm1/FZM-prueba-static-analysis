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

    AppConfig cfg = {0};
    uint32_t crc_stored = 0;
    load_config((argc > 1) ? argv[1] : "tests/example.cfg", &cfg, &crc_stored);

    RingBuffer rb;
    rb_init(&rb, 16);

    pthread_t th_prod, th_cons;
    
    pthread_create(&th_cons, NULL, consumer, &rb);
    pthread_create(&th_prod, NULL, producer, &rb);

    uint8_t serialized[sizeof(ConfigCrcView)];
    int len = config_crc_view_serialize(&cfg, serialized, sizeof(serialized));
    uint32_t crc_calc = crc32_compute(serialized, len);

    if (crc_stored != crc_calc){
        printf("Invalid CRC: saved=%08x, stored=%08x\n", crc_stored, crc_calc);
        return -1;
    }
    printf("CRC correct: saved=%08x, stored=%08x\n", crc_stored, crc_calc);

    pthread_join(th_prod, NULL);
    pthread_join(th_cons, NULL);

    rb_free(&rb);
    rb_free(&rb);

    return 0;
}
