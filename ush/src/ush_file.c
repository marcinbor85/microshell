#include "ush_file.h"
#include "ush_utils.h"
#include "ush_node.h"

#include <string.h>

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        char file_path[USH_CONFIG_PATH_MAX_LENGTH];
        char abs_path[USH_CONFIG_PATH_MAX_LENGTH];

        struct ush_node_object *curr;
        struct ush_file_descriptor const *file;

        ush_node_get_absolute_path(self, name, abs_path);

        curr = self->commands;        
        while (curr != NULL) {
                for (size_t i = 0; i < curr->file_list_size; i++) {
                        file = &curr->file_list[i];
                        if (strcmp(file->name, name) == 0)
                                return file;
                }
                curr = curr->next;                
        }

        curr = ush_node_get_parent_by_path(self, abs_path);      
        while (curr != NULL) {
                for (size_t i = 0; i < curr->file_list_size; i++) {
                        file = &curr->file_list[i];
                        ush_utils_join_path(curr->path, file->name, file_path);
                        if (strcmp(file_path, abs_path) == 0)
                                return file;
                }
                curr = curr->next;                
        }

        return NULL;
}
