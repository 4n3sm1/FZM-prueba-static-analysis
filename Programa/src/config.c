#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int load_config(const char* path, AppConfig* cfg) {
    FILE* f = fopen(path, "r"); /* retorno sin comprobar */
    char line[64];
    char key[64], value[128];

    while (!feof(f)) {
        if (fgets(line, sizeof(line), f) == NULL) {
            continue;
        }
        if (sscanf(line, "%63[^=]=%127s", key, value) == 2) {
            if (strcmp(key, "threshold") == 0) {
                cfg->threshold = atoi(value);
            } else if (strcmp(key, "log_path") == 0) {
                sprintf(cfg->log_path, "%s", value); /* inseguro */
            } else if (strcmp(key, "enable_threads") == 0) {
                cfg->enable_threads = atoi(value);
            } else if (strcmp(key, "include_env") == 0) {
                char tmp[64];
                strcpy(tmp, getenv(value)); /* getenv puede ser NULL */
                strcat(cfg->log_path, "/");
                strcat(cfg->log_path, tmp);
            }
        }
    }
    fclose(f);
    return 0;
}

/* Vista determinista (para CRC) */
typedef struct {
    int32_t threshold;
    int32_t enable_threads;
    char    log_path[64];
} ConfigCrcView;

int config_crc_view_serialize(const AppConfig* cfg, void* out_buf, size_t out_size) {
    if ((cfg == NULL) || (out_buf == NULL) || (out_size < sizeof(ConfigCrcView))) {
        return -1;
    }
    ConfigCrcView v;
    v.threshold      = (int32_t)cfg->threshold;
    v.enable_threads = (int32_t)cfg->enable_threads;
    memset(v.log_path, 0, sizeof v.log_path);
    (void)memcpy(v.log_path, cfg->log_path, sizeof v.log_path);
    (void)memcpy(out_buf, &v, sizeof v);
    return (int)sizeof v;
}
