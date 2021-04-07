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

void ush_path_mount(struct ush_object *self, const char *mount_point, const char *name, struct ush_path_object *path_obj, const struct ush_cmd_descriptor *cmd_list, size_t cmd_list_size);
void ush_path_unmount(struct ush_object *self, struct ush_path_object *path_obj);
bool ush_path_set_current_dir(struct ush_object *self, const char *path);
void ush_path_get_current_dir(struct ush_object *self, char *path, size_t max_len);
void ush_path_get_full_path(struct ush_object *self, struct ush_path_object *path_obj, char *full_path, size_t max_len);

void ush_cmd_get_full_path(struct ush_object *self, struct ush_path_object *path_obj, const char *name, char *path, size_t max_len);
struct ush_cmd_descriptor const* ush_cmd_find_by_name(struct ush_object *self, const char *name);

const char* ush_message_get_string(struct ush_object *self, ush_message_t msg);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
