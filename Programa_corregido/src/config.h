#ifndef CONFIG_H
#define CONFIG_H
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int threshold;
    char log_path[64];
    int enable_threads;
} AppConfig;

int load_config(const char* path, AppConfig* cfg);

/* Extensión de integridad (vista determinista opcional) */
int config_crc_view_serialize(const AppConfig* cfg, void* out_buf, size_t out_size);

#endif
