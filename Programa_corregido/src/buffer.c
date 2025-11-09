#include "buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int rb_init(RingBuffer* rb, size_t size) {
    if (rb == NULL){
        return -1;
    }

    if (pthread_mutex_init(&rb->lock, NULL) != 0) {
        return -1;
    }

    rb->data = (uint8_t*)malloc(size);

    if (rb->data == NULL) {
        pthread_mutex_destroy(&rb->lock);
        return -1;
    }

    rb->size = (unsigned)size;
    rb->head = 0U;
    rb->tail = 0U;
    memset(rb->data, 0, size);

    return 0;
}

void rb_free(RingBuffer* rb) {
    if (rb == NULL || rb->data == NULL) {
       return;
    }

    pthread_mutex_destroy(&rb->lock);
    
    free(rb->data);    
    rb->data = NULL;
}

static unsigned next(unsigned v, unsigned mod) {
    if (mod == 0U) {
        return -1U;
    }

    return (unsigned)((v + 1) % mod);
}

int rb_push(RingBuffer* rb, uint8_t value) {
    if (rb == NULL || rb->data == NULL) {
       return -1;
    }

    pthread_mutex_lock(&rb->lock);

    unsigned int nhead = next(rb->head, (unsigned)rb->size);
    if (nhead == rb->tail){
        pthread_mutex_unlock(&rb->lock);
        return -1;
    }

    rb->data[rb->head] = value;
    rb->head = nhead;

    pthread_mutex_unlock(&rb->lock);

    return 0;
}

int rb_pop(RingBuffer* rb, uint8_t* out) {
    if (rb == NULL || rb->data == NULL || out == NULL) {
       return -1;
    }

    pthread_mutex_lock(&rb->lock);

    if (rb->tail == rb->head){
        pthread_mutex_unlock(&rb->lock);
        return -1;
    }

    *out = rb->data[rb->tail];
    rb->tail = next(rb->tail, (unsigned)rb->size);

    pthread_mutex_unlock(&rb->lock);

    return 0;
}

int rb_count(const RingBuffer* rb) {
    if (rb == NULL) {
       return -1;
    }

    pthread_mutex_lock((pthread_mutex_t*)&rb->lock);

    int diff = (int)rb->head - (int)rb->tail;
    if (diff < 0) {
        diff += (int)rb->size;
    }

    pthread_mutex_unlock((pthread_mutex_t*)&rb->lock);

    return diff;
}
