#include "ush_node.h"
#include "ush_utils.h"

#include <string.h>

ush_status_t ush_node_mount(struct ush_object *self, const char *path, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(node != NULL);
        USH_ASSERT(path != NULL);

        for (size_t i = 0; i < file_list_size; i++) {
                USH_ASSERT(file_list[i].name != NULL);
        }

        node->file_list = file_list;
        node->file_list_size = file_list_size;
        node->path = path;

        struct ush_node_object *node_exists = ush_node_get_by_path(self, path);
        if (node_exists != NULL)
                return USH_STATUS_ERROR_NODE_ALREADY_MOUNTED;

        struct ush_node_object *node_parent = ush_node_get_parent_by_path(self, path);
        if (node_parent != NULL) {
                node->next = node_parent->childs;
                node_parent->childs = node;
                node->parent = node_parent;
                return USH_STATUS_OK;
        }

        if (strcmp(path, "/") == 0) {
                node->next = NULL;
                self->root = node;
                node->parent = NULL;
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

        if ((node == NULL) || ((node != self->root) && (parent_node == NULL)))
                return USH_STATUS_ERROR_NODE_NOT_FOUND;
        
        if (node->childs != NULL)
                return USH_STATUS_ERROR_NODE_WITH_CHILDS;

        if (parent_node == NULL) {
                self->root = NULL;
                return USH_STATUS_OK;
        }

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

        return USH_STATUS_ERROR_NODE_NOT_FOUND;
}
