#ifndef USH_INTERNAL_H
#define USH_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

void ush_read_start(struct ush_object *self);
bool ush_read_char(struct ush_object *self);
void ush_read_echo_service(struct ush_object *self, char ch);
bool ush_read_char_by_escape_state(struct ush_object *self, char ch);
bool ush_read_service(struct ush_object *self, bool *read);

void ush_parse_start(struct ush_object *self);
void ush_parse_char(struct ush_object *self);
void ush_parse_char_standard(struct ush_object *self, char ch);
void ush_parse_finish(struct ush_object *self);
bool ush_parse_service(struct ush_object *self);
int ush_parse_get_args(struct ush_object *self, char* *argv);

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t write_next_state);
void ush_write_pointer_bin(struct ush_object *self, uint8_t *data, size_t data_size, ush_state_t write_next_state);
void ush_write_char(struct ush_object *self);
bool ush_write_service(struct ush_object *self);

void ush_reset_start(struct ush_object *self);
bool ush_reset_service(struct ush_object *self);

void ush_prompt_start(struct ush_object *self, ush_state_t prepare_next_state);
bool ush_prompt_service(struct ush_object *self);

void ush_process_start(struct ush_object *self, const struct ush_file_descriptor *file);
bool ush_process_service(struct ush_object *self);

#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1      

void ush_autocomp_start(struct ush_object *self);
bool ush_autocomp_service(struct ush_object *self);
void ush_autocomp_prepare_candidates(struct ush_object *self);
void ush_autocomp_optimize_continue(struct ush_object *self);
void ush_autocomp_check_for_finish(struct ush_object *self);
void ush_autocomp_state_prepare(struct ush_object *self);
void ush_autocomp_state_candidates_start(struct ush_object *self);
void ush_autocomp_state_candidates_process(struct ush_object *self);
void ush_autocomp_state_candidates_finish(struct ush_object *self);
void ush_autocomp_state_recall_suffix(struct ush_object *self);

#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */


#ifdef __cplusplus
}
#endif

#endif /* USH_INTERNAL_H */
