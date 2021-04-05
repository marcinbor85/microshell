
#ifndef USH_TYPES_H
#define USH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ush_config.h"

typedef enum {
        USH_STATE_RESET,
        USH_STATE_READ_PREPARE,
        USH_STATE_READ_CHAR,
        USH_STATE_PARSE_SEARCH_ARG,
        USH_STATE_PARSE_QUOTE_ARG,
        USG_STATE_PARSE_STANDARD_ARG,
        USH_STATE_PARSE_SEARCH_STOP,
        USH_STATE_WRITE_CHAR,
} ush_state_t;

struct ush_object;

typedef int (*ush_iface_read_char)(struct ush_object *self, char *ch);
typedef int (*ush_iface_write_char)(struct ush_object *self, char ch);

struct ush_iface {
        ush_iface_read_char read;
        ush_iface_write_char write;
};

struct ush_descriptor {
        struct ush_iface const *iface;
        char *input_buffer;
        size_t input_buffer_size;
        char *output_buffer;
        size_t output_buffer_size;
        char *hostname;
};

struct ush_object {
        struct ush_descriptor const *desc;

        ush_state_t state;
        ush_state_t next_write_state;

        size_t output_size;
        size_t input_position;
        size_t output_position;
        size_t argc;
        bool escape;

        char current_dir[USH_CONFIG_CURRENT_DIR_MAX_SIZE];
};

#ifdef __cplusplus
}
#endif

#endif /* USH_TYPES_H */
