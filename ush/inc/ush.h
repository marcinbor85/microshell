#ifndef USH_H
#define USH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"
#include "ush_config.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc);
bool ush_service(struct ush_object *self);
void ush_reset(struct ush_object *self);
void ush_path_register(struct ush_object *self, const char *path, struct ush_path_object *path_obj, const struct ush_cmd_descriptor *desc, size_t desc_num);
void ush_path_unregister(struct ush_object *self, struct ush_path_object *path_obj);

struct ush_cmd_descriptor const* ush_cmd_find_by_path(struct ush_object *self, const char *path);
struct ush_cmd_descriptor const* ush_cmd_find_by_name_in_current_dir(struct ush_object *self, const char *name);

const char* ush_message_get_string(struct ush_object *self, ush_message_t msg);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
