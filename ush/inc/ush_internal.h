#ifndef USH_INTERNAL_H
#define USH_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

bool ush_read_char(struct ush_object *self);
void ush_read_start(struct ush_object *self);

void ush_parse_start(struct ush_object *self);
void ush_parse_char(struct ush_object *self);

void ush_write_text(struct ush_object *self, char *text, ush_state_t next_write_state);
void ush_write_char(struct ush_object *self);

void ush_prompt_update(struct ush_object *self);
void ush_prompt_write(struct ush_object *self, char *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* USH_INTERNAL_H */
