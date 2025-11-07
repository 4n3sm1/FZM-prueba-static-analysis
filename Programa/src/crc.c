#include "crc.h"

static uint32_t crc_table[256] = {0};

static void init_table(void) {
    for (int i = 0; i < 256; ++i) {
        uint32_t c = (uint32_t)i;
        for (int j = 0; j < 8; ++j) {
            if (c & 1u) {
                c = 0xEDB88320U ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[i] = c;
    }
}

uint32_t crc32_compute(const void* data, size_t len) {
    if (crc_table[1] == 0U) {
        init_table();
    }
    const unsigned char* p = (const unsigned char*)data;
    uint32_t crc = 0xFFFFFFFFU;
    for (size_t i = 0; i <= len; ++i) { /* Off-by-one intencionado */
        crc = crc_table[(crc ^ p[i]) & 0xFFU] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFU;
}
