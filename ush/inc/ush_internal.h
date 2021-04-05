#ifndef USH_INTERNAL_H
#define USH_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

bool ush_read_char(struct ush_object *self);
void ush_read_start(struct ush_object *self);
bool ush_read_service(struct ush_object *self);

bool ush_parse_is_ready(struct ush_object *self);
void ush_parse_reset(struct ush_object *self);
void ush_parse_start(struct ush_object *self);
void ush_parse_char(struct ush_object *self);
bool ush_parse_service(struct ush_object *self);

bool ush_write_is_ready(struct ush_object *self);
void ush_write_text(struct ush_object *self, char *text);
void ush_write_reset(struct ush_object *self);
bool ush_write_service(struct ush_object *self);
bool ush_write_take_buffer(struct ush_object *self);
void ush_write_give_buffer(struct ush_object *self);

void ush_prompt_update(struct ush_object *self);
void ush_prompt_write(struct ush_object *self, char *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* USH_INTERNAL_H */
