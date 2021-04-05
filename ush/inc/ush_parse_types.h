
#ifndef USH_PARSE_TYPES_H
#define USH_PARSE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
        USH_PARSE_STATE_IDLE,
        USH_PARSE_STATE_WAIT_BUFFER,
        USH_PARSE_STATE_SEARCH_ARG,
        USH_PARSE_STATE_QUOTE_ARG,
        USG_PARSE_STATE_STANDARD_ARG,
        USH_PARSE_STATE_SEARCH_STOP,
        USH_PARSE_STATE_WAIT_PROCESS,
} ush_parse_state_t;

struct ush_parse_fsm {
        ush_parse_state_t state;
        size_t input_position;
        size_t output_position;
        size_t argc;
        bool escape;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_PARSE_TYPES_H */
