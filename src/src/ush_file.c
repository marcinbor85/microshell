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

#include "inc/ush_file.h"
#include "inc/ush_utils.h"
#include "inc/ush_node.h"

#include <string.h>

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        char file_path[self->desc->path_max_length];
        char abs_path[self->desc->path_max_length];

        struct ush_node_object *curr;
        struct ush_file_descriptor const *file;

        curr = self->commands;        
        while (curr != NULL) {
                for (size_t i = 0; i < curr->file_list_size; i++) {
                        file = &curr->file_list[i];
                        if (strcmp(file->name, name) == 0)
                                return file;
                }
                curr = curr->next;                
        }

        ush_node_get_absolute_path(self, name, abs_path);
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
