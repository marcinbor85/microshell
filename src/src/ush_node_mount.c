/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "inc/ush_node.h"
#include "inc/ush_utils.h"

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
                node->next = node_parent->children;
                node_parent->children = node;
                node->parent = node_parent;
                return USH_STATUS_OK;
        }

        if (strcmp(path, "/") == 0) {
                node->next = NULL;
                self->root = node;
                self->current_node = self->root;
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
        
        if (node->children != NULL)
                return USH_STATUS_ERROR_NODE_WITH_CHILDS;

        if (parent_node == NULL) {
                self->root = NULL;
                return USH_STATUS_OK;
        }

        struct ush_node_object *prev = NULL;
        struct ush_node_object *curr = parent_node->children;

        while (curr != NULL) {
                if (curr != node) {
                        prev = curr;
                        curr = curr->next;
                        continue;
                }
                if (prev == NULL) {
                        parent_node->children = node->next;
                } else {
                        prev->next = node->next;
                }
                return USH_STATUS_OK;           
        }

        return USH_STATUS_ERROR_NODE_NOT_FOUND;
}
