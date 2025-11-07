#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "buffer.h"
#include "crc.h"
#include "config.h"

int main(void) {
    RingBuffer rb;
    rb_init(&rb, 4);
    assert(rb_push(&rb, 1) == 0);
    assert(rb_push(&rb, 2) == 0);
    uint8_t out = 0xFF;
    assert(rb_pop(&rb, &out) == 0);
    printf("out=%u\n", (unsigned)out);

    const char* s = "abc";
    uint32_t c = crc32_compute(s, 3);
    printf("crc=%08x\n", c);

    AppConfig cfg;
    int r = load_config("tests/example.cfg", &cfg);
    (void)r;
    printf("cfg threshold=%d enable_threads=%d log_path=%s\n", cfg.threshold, cfg.enable_threads, cfg.log_path);

    return 0;
}
