#include "ush.h"

#include <string.h>

void ush_file_get_full_path(struct ush_object *self, struct ush_node_object *path_obj, const char *name, char *path, size_t max_len)
{
        (void)self;
        (void)path_obj;
        (void)max_len;

       // ush_path_get_full_path(self, path_obj, path, max_len);
        
        if (strcmp(path, "/") == 0) {
                strcat(path, name);
        } else {
                strcat(path, "/");
                strcat(path, name);
        }
}

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        char path[USH_CONFIG_CURRENT_DIR_MAX_SIZE];
        char abs_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE];

        struct ush_node_object *curr;
        struct ush_file_descriptor const *cmd;

        ush_path_get_absolute_path(self, name, abs_path, sizeof(abs_path));
        // strcpy(path, abs_path);
        // ush_utils_path_last(path);

        curr = self->commands;        
        while (curr != NULL) {
                for (size_t i = 0; i < curr->file_list_size; i++) {
                        cmd = &curr->file_list[i];
                        if (strcmp(cmd->name, name) == 0)
                                return cmd;
                }
                curr = curr->next;                
        }

        curr = self->root;        
        while (curr != NULL) {
                for (size_t i = 0; i < curr->file_list_size; i++) {
                        cmd = &curr->file_list[i];
                        ush_file_get_full_path(self, curr, cmd->name, path, sizeof(path));
                        if (strcmp(path, abs_path) == 0)
                                return cmd;
                }
                curr = curr->next;                
        }

        return NULL;
}
