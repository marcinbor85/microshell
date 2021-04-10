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

bool ush_node_mount(struct ush_object *self, const char *path, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size);
bool ush_node_unmount(struct ush_object *self, const char *path);
bool ush_node_set_current_dir(struct ush_object *self, const char *path);
void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *abs_path);
struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path);
struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path);

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name);

const char* ush_message_get_string(struct ush_object *self, ush_message_t msg);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
