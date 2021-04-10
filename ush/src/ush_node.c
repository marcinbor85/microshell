#include "ush_node.h"
#include "ush_utils.h"

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

ush_status_t ush_node_mount(struct ush_object *self, const char *path, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size)
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
                return USH_STATUS_OK;
        }
        
        struct ush_node_object *node_exists = ush_node_get_by_path(self, path);
        if (node_exists != NULL)
                return USH_STATUS_ERROR_NODE_ALREADY_MOUNTED;

        struct ush_node_object *node_parent = ush_node_get_parent_by_path(self, path);
        if (node_parent != NULL) {
                node->next = node_parent->childs;
                node_parent->childs = node;
                return USH_STATUS_OK;
        }

        if (strcmp(path, "/") == 0) {
                node->next = NULL;
                self->root = node;
                return USH_STATUS_OK;
        }

        return USH_STATUS_ERROR_NODE_WITHOUT_PARENT;
}

ush_status_t ush_node_unmount(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        struct ush_node_object *parent_node = ush_node_get_parent_by_path(self, path);
        struct ush_node_object *node = ush_node_get_by_path(self, path);

        if ((node == NULL) || (parent_node == NULL))
                return USH_STATUS_ERROR_NODE_NOT_EXISTS;
        
        if (node->childs != NULL)
                return USH_STATUS_ERROR_NODE_WITH_CHILDS;

        struct ush_node_object *prev = NULL;
        struct ush_node_object *curr = parent_node->childs;

        while (curr != NULL) {
                if (curr != node) {
                        prev = curr;
                        curr = curr->next;
                        continue;
                }
                if (prev == NULL) {
                        parent_node->childs = node->next;
                } else {
                        prev->next = node->next;
                }
                return USH_STATUS_OK;           
        }

        return USH_STATUS_ERROR_NODE_NOT_EXISTS;
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

ush_status_t ush_node_set_current_dir(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char abs_path[USH_CONFIG_PATH_MAX_LENGTH];

        ush_node_get_absolute_path(self, path, abs_path);

        if (path[0] == '\0')
                return USH_STATUS_ERROR_NODE_NOT_EXISTS;

        struct ush_node_object *node = ush_node_get_by_path(self, abs_path);
        if (node != NULL) {
                self->current_node = node;
                return USH_STATUS_OK;
        }

        return USH_STATUS_ERROR_NODE_NOT_EXISTS;
}
