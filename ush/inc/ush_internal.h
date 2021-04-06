#ifndef USH_INTERNAL_H
#define USH_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

void ush_read_start(struct ush_object *self);
bool ush_read_char(struct ush_object *self);
void ush_read_echo_service(struct ush_object *self, char ch);
bool ush_read_service(struct ush_object *self, bool *read);

void ush_parse_start(struct ush_object *self);
void ush_parse_char(struct ush_object *self);
bool ush_parse_service(struct ush_object *self);

void ush_write_copy(struct ush_object *self, char *text, ush_state_t write_next_state);
void ush_write_pointer(struct ush_object *self, char *text, ush_state_t write_next_state);
void ush_write_char(struct ush_object *self);
bool ush_write_service(struct ush_object *self);

void ush_reset_start(struct ush_object *self);
bool ush_reset_service(struct ush_object *self);

void ush_prompt_start(struct ush_object *self);
bool ush_prompt_service(struct ush_object *self);

struct ush_cmd_object* ush_cmd_find_by_name(struct ush_object *self, char *name);

const char* ush_message_get(struct ush_object *self, ush_message_t msg);

#ifdef __cplusplus
}
#endif

#endif /* USH_INTERNAL_H */
