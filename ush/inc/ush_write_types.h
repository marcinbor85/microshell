
#ifndef USH_WRITE_TYPES_H
#define USH_WRITE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
        USH_WRITE_STATE_IDLE,
        USH_WRITE_STATE_PROCESS,
} ush_write_state_t;

struct ush_write_fsm {
        ush_write_state_t state;
        size_t size;
        size_t position;
        bool mutex;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_WRITE_TYPES_H */
