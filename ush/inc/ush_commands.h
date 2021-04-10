#ifndef USH_COMMANDS_H
#define USH_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

extern const struct ush_file_descriptor g_ush_buildin_commands[];
extern size_t g_ush_buildin_commands_num;

ush_status_t ush_commands_add(struct ush_object *self, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size);
ush_status_t ush_commands_remove(struct ush_object *self, struct ush_node_object *node);

#ifdef __cplusplus
}
#endif

#endif /* USH_COMMANDS_H */
