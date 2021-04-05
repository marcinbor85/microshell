
#ifndef USH_TYPES_H
#define USH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ush_config.h"

#include "ush_iface_types.h"
#include "ush_read_types.h"
#include "ush_parse_types.h"
#include "ush_write_types.h"

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

        struct ush_read_fsm read;
        struct ush_parse_fsm parse;
        struct ush_write_fsm write;

        char current_dir[USH_CONFIG_CURRENT_DIR_MAX_SIZE];
};

#ifdef __cplusplus
}
#endif

#endif /* USH_TYPES_H */
