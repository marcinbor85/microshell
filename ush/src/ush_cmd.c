#include "ush.h"

#include <string.h>

struct ush_cmd_descriptor const* ush_cmd_find_by_path(struct ush_object *self, const char *path)
{
        struct ush_path_object *curr = self->path_first;
        char cmd_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];
        
        while (curr != NULL) {
                for (size_t i = 0; i < curr->cmd_list_size; i++) {
                        struct ush_cmd_descriptor const *cmd = &curr->cmd_list[i];
                        snprintf(cmd_path, sizeof(cmd_path), "%s/%s", self->current_dir, cmd->name);
                        if (strcmp(cmd_path, path) == 0)
                                return cmd;
                }
                
                curr = curr->next;
        }

        return NULL;
}

struct ush_cmd_descriptor const* ush_cmd_find_by_name_in_current_dir(struct ush_object *self, const char *name)
{
        char cmd_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];

        snprintf(cmd_path, sizeof(cmd_path), "%s/%s", self->current_dir, name);
        
        return ush_cmd_find_by_path(self, cmd_path);
}
