#include "ush_internal.h"
#include "ush.h"

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

struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char level_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE];
        
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

bool ush_node_mount(struct ush_object *self, const char *path, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(node != NULL);
        USH_ASSERT(file_list != NULL);
        USH_ASSERT(file_list_size > 0);

        for (size_t i = 0; i < file_list_size; i++) {
                USH_ASSERT(file_list[i].name != NULL);
        }

        node->file_list = file_list;
        node->file_list_size = file_list_size;
        node->path = path;

        if (path == NULL) {
                node->next = self->commands;
                self->commands = node;
                return true;
        }
        
        /* TODO: add protection from multiple same paths */

        struct ush_node_object *node_parent = ush_node_get_parent_by_path(self, path);
        if (node_parent != NULL) {
                node->next = node_parent->childs;
                node_parent->childs = node;
                return true;
        }

        if (strcmp(path, "/") == 0) {
                node->next = NULL;
                self->root = node;
                return true;
        }

        return false;
}

bool ush_node_unmount(struct ush_object *self, const char *path)
{
        (void)self;
        (void)path;
        /* TODO: add unmount by path implementation */

        // USH_ASSERT(self != NULL);
        // USH_ASSERT(node != NULL);

        // struct ush_node_object *prev = NULL;
        // struct ush_node_object *curr = self->root;

        // while (curr != NULL) {
        //         if (curr != node) {
        //                 prev = curr;
        //                 curr = curr->next;
        //                 continue;
        //         }
        //         if (prev == NULL) {
        //                 self->root = node->next;
        //         } else {
        //                 prev->next = node->next;
        //         }
        //         break;                
        // }

        return false;
}





void ush_path_get_absolute_path(struct ush_object *self, const char *in_path, char *out_path, size_t max_len)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(out_path != NULL);
        USH_ASSERT(max_len > 0);

        char curr_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE*2];
        char abs_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE*3];

        if (in_path[0] == '/') {
                strncpy(abs_path, in_path, sizeof(abs_path));
                abs_path[sizeof(abs_path) - 1] = '\0';
        } else {
                strncpy(curr_path, self->current_node->path, sizeof(curr_path));
                curr_path[sizeof(curr_path) - 1] = '\0';
                if (strcmp(curr_path, "/") == 0) {
                        snprintf(abs_path, sizeof(abs_path), "/%s", in_path);
                } else {
                        snprintf(abs_path, sizeof(abs_path), "%s/%s", curr_path, in_path);
                }
        }

        size_t abs_path_len = strlen(abs_path);
        if ((abs_path_len > 1) && (abs_path[abs_path_len - 1] == '/'))
                abs_path[abs_path_len - 1] = '\0';

        ush_utils_get_collapse_path(abs_path, out_path, max_len);
}

bool ush_path_set_current_dir(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char abs_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];

        ush_path_get_absolute_path(self, path, abs_path, sizeof(abs_path));

        bool success = false;

        if (path[0] == '\0')
                return false;

        struct ush_node_object *node = ush_node_get_by_path(self, abs_path);
        if (node != NULL) {
                self->current_node = node;
                success = true;
        }

        return success;
}
