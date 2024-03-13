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

struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char level_path[self->desc->path_max_length];
        
        size_t levels = ush_utils_get_path_levels_count(path);

        if (levels == 0)
                return self->root;
        if (self->root == NULL)
                return NULL;

        struct ush_node_object *curr = self->root->children;
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

                if (found == false)
                        break;
                
                if (i == levels)
                        return curr;

                curr = curr->children;
        }
        

        return NULL;
}

void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *out_path)
{
        char abs_path[self->desc->path_max_length];

        USH_ASSERT(self != NULL);
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(out_path != NULL);

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

void ush_node_deinit_recursive(struct ush_object *self, struct ush_node_object *node)
{
        USH_ASSERT(self != NULL);

        struct ush_node_object *curr = node;

        while (curr != NULL) {
                if (curr->children != NULL)
                        ush_node_deinit_recursive(self, curr->children);

                struct ush_node_object *tmp = curr->next;
                memset((uint8_t*)curr, 0, sizeof(struct ush_node_object));
                curr = tmp;
        }
}
