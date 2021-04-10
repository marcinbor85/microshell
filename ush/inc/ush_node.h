#ifndef USH_NODE_H
#define USH_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

ush_status_t ush_node_mount(struct ush_object *self, const char *path, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size);
ush_status_t ush_node_unmount(struct ush_object *self, const char *path);
ush_status_t ush_node_set_current_dir(struct ush_object *self, const char *path);

void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *abs_path);

struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path);
struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* USH_NODE_H */
