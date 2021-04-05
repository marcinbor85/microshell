
#ifndef USH_READ_TYPES_H
#define USH_READ_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
        USH_READ_STATE_READ,
        USH_READ_STATE_WAIT_PARSE,
        USH_READ_STATE_WAIT_PARSE_END,
} ush_read_state_t;

struct ush_read_fsm {
        ush_read_state_t state;
        size_t input_buffer_index;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_READ_TYPES_H */
