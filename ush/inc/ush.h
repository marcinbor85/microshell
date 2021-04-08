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

void ush_path_mount(struct ush_object *self, const char *mount_point, const char *name, struct ush_path_object *path_obj, const struct ush_file_descriptor *file_list, size_t file_list_size);
void ush_path_unmount(struct ush_object *self, struct ush_path_object *path_obj);
bool ush_path_set_current_dir(struct ush_object *self, const char *path);
void ush_path_get_current_dir(struct ush_object *self, char *path, size_t max_len);
void ush_path_get_full_path(struct ush_object *self, struct ush_path_object *path_obj, char *full_path, size_t max_len);
void ush_path_get_absolute_path(struct ush_object *self, const char *in_path, char *abs_path, size_t max_len);
struct ush_path_object * ush_path_by_mount_point(struct ush_object *self, const char *mount_point);
size_t ush_path_get_levels_count(struct ush_object *self, const char *path);
void ush_path_get_collapse(struct ush_object *self, char *in_path, char *path, size_t max_len);

void ush_file_get_full_path(struct ush_object *self, struct ush_path_object *path_obj, const char *name, char *path, size_t max_len);
struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name);

const char* ush_message_get_string(struct ush_object *self, ush_message_t msg);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
