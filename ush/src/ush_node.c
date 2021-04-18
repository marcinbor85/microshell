#include "ush_node.h"
#include "ush_utils.h"

#include <string.h>

struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char level_path[USH_CONFIG_PATH_MAX_LENGTH];
        
        size_t levels = ush_utils_get_path_levels_count(path);

        if (levels == 0)
                return self->root;
        if (self->root == NULL)
                return NULL;

        struct ush_node_object *curr = self->root->childs;
        for (size_t i = 1; i <= levels; i++) {
                ush_utils_get_path_level(i, path, level_path);
                
                bool found = false;
                while (curr != NULL) {
                        if (strcmp(curr->path, level_path) == 0) {
                                found = true;
                                break;
                        }
                        curr = curr->next;
                }

                if (i == levels) {
                        if (found == false) {
                                return NULL;
                        } else {
                                return curr;
                        }
                }

                if (found == false) {
                        return NULL;
                } else {
                        curr = curr->childs;
                }
        }
        

        return NULL;
}

void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *out_path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(out_path != NULL);

        char abs_path[USH_CONFIG_PATH_MAX_LENGTH];

        if (in_path[0] == '/') {
                strcpy(abs_path, in_path);
        } else {
                strcpy(abs_path, self->current_node->path);
                if (strcmp(self->current_node->path, "/") != 0)
                        strcat(abs_path, "/");
                strcat(abs_path, in_path);
        }

        size_t abs_path_len = strlen(abs_path);
        if ((abs_path_len > 1) && (abs_path[abs_path_len - 1] == '/'))
                abs_path[abs_path_len - 1] = '\0';

        ush_utils_get_collapse_path(abs_path, out_path);
}
