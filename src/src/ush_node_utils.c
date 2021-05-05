#include "inc/ush_node.h"
#include "inc/ush_utils.h"

#include <string.h>

struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        size_t path_len = strlen(path);
        char path_parent[path_len + 1];
        memcpy(path_parent, path, path_len + 1);

        ush_utils_path_upper(path_parent);
        if (path_parent[0] == '\0')
                return NULL;
        
        return ush_node_get_by_path(self, path_parent);
}

ush_status_t ush_node_set_current_dir(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        if (path[0] == '\0')
                return USH_STATUS_ERROR_NODE_NOT_FOUND;

        char abs_path[self->desc->path_max_length];
        
        ush_node_get_absolute_path(self, path, abs_path);
        struct ush_node_object *node = ush_node_get_by_path(self, abs_path);
        if (node != NULL) {
                self->current_node = node;
                return USH_STATUS_OK;
        }

        return USH_STATUS_ERROR_NODE_NOT_FOUND;
}
