#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

typedef struct {
    uint8_t *data;
    size_t size;
    volatile unsigned head;
    volatile unsigned tail;
    pthread_mutex_t lock;
} RingBuffer;

int rb_init(RingBuffer* rb, size_t size);
void rb_free(RingBuffer* rb);
int rb_push(RingBuffer* rb, uint8_t value);
int rb_pop(RingBuffer* rb, uint8_t* out);
int rb_count(const RingBuffer* rb);

#endif /* BUFFER_H */
