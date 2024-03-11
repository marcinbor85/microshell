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

#include "inc/ush_internal.h"
#include "inc/ush_preconfig.h"
#include "inc/ush_utils.h"
#include "inc/ush_node.h"

#include <string.h>

#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1

void ush_autocomp_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->state = USH_STATE_AUTOCOMP_PREPARE;
}        

void ush_autocomp_prepare_candidates(struct ush_object *self)
{
        self->autocomp_count = 0;
        self->process_stage = 0;
        self->process_node = self->commands;
        self->process_index = 0;
        self->process_index_item = 0;
}

void ush_autocomp_optimize_continue(struct ush_object *self)
{
        self->autocomp_prev_count = self->autocomp_count;

        size_t pos = strlen(self->autocomp_input);
        if (pos >= strlen(self->autocomp_candidate_name)) {
                self->state = USH_STATE_AUTOCOMP_RECALL_SUFFIX;
                return;
        }
        char ch = self->autocomp_candidate_name[pos];
        self->desc->input_buffer[self->in_pos++] = ch;
        if (self->in_pos >= self->desc->input_buffer_size)
                self->in_pos = 0;
        self->desc->input_buffer[self->in_pos] = '\0';
        self->autocomp_suffix_len++;

        ush_autocomp_prepare_candidates(self);
        self->state = USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE;
}

void ush_autocomp_check_for_finish(struct ush_object *self)
{
        char abs_path[self->desc->path_max_length];

        if (self->process_stage == 0) {
                self->process_node = self->current_node;
                self->process_index = 0;
                self->process_index_item = 0;
                self->process_stage = 1;
        } else if (self->process_stage == 1) {
                ush_node_get_absolute_path(self, self->autocomp_input, abs_path);
                if (self->autocomp_input[0] == '\0') {
                        self->process_node = self->current_node;
                } else {
                        self->process_node = ush_node_get_parent_by_path(self, abs_path);
                        if (self->process_node == NULL)
                                self->process_node = self->current_node;
                }

                self->process_node = self->process_node->children;
                self->process_index = 0;
                self->process_index_item = 0;
                self->process_stage = 2;
        } else if (self->process_stage == 2) {
                self->autocomp_prev_state = self->state;
                self->state = USH_STATE_AUTOCOMP_CANDIDATES_FINISH;
        } else {
                USH_ASSERT(false);
        }
}

bool ush_autocomp_check_for_next(struct ush_object *self)
{
        bool ret = false;

        if (self->process_stage == 0) {
                if (self->process_index_item >= self->process_node->file_list_size) {
                        self->process_index_item = 0;
                        self->process_node = self->process_node->next;
                        ret = true;
                }
        } else if (self->process_stage == 1) {
                if (self->process_index_item >= self->process_node->file_list_size) {
                        self->process_index_item = 0;
                        self->process_node = NULL;
                        ret = true;
                }
        } else if (self->process_stage == 2) {
                /* do nothing */
        } else {
                USH_ASSERT(false);
        }

        return ret;
}

void ush_autocomp_process_file_index(struct ush_object *self, const struct ush_file_descriptor *file)
{
        switch (self->process_index) {
        case 0:
                if ((self->process_stage == 0) || (self->process_stage == 1)) {
                        if (self->state == USH_STATE_AUTOCOMP_CANDIDATES_PRINT)
                                ush_write_pointer(self, (char*)file->name, self->state);
                        self->process_index = 1;
                        self->autocomp_count++;
                        self->autocomp_candidate_name = (char*)file->name;
                } else if (self->process_stage == 2) {
                        if (self->state == USH_STATE_AUTOCOMP_CANDIDATES_PRINT)
                                ush_write_pointer(self, self->autocomp_name, self->state);
                        self->process_index = 1;
                        self->autocomp_count++;
                        self->autocomp_candidate_name = self->autocomp_name;
                } else {
                        USH_ASSERT(false);
                }               
                break;
        case 1:
                if (self->state == USH_STATE_AUTOCOMP_CANDIDATES_PRINT)
                        ush_write_pointer(self, "\r\n", self->state);
                self->process_index = 2;
                break;
        case 2:
                if ((self->process_stage == 0) || (self->process_stage == 1)) {
                        self->process_index_item++;
                        self->process_index = 0;
                } else if (self->process_stage == 2) {
                        self->process_node = self->process_node->next;
                        self->process_index = 0;
                } else {
                        USH_ASSERT(false);
                }                        
                break;
        default:
                USH_ASSERT(false);
                break;
        }
}

bool ush_autocomp_process_file_prepare(struct ush_object *self, struct ush_file_descriptor const **file)
{
        bool ret = false;

        if ((self->process_stage == 0) || (self->process_stage == 1)) {
                *file = &self->process_node->file_list[self->process_index_item];
                if (ush_utils_startswith((char*)((*file)->name), self->autocomp_input) == false) {
                        self->process_index_item++;
                        self->process_index = 0;
                        ret = true;
                }
        } else if (self->process_stage == 2) {
                self->autocomp_name = ush_utils_path_last(self->process_node->path);
                if (ush_utils_startswith(self->autocomp_name, self->autocomp_input) == false) {
                        self->process_node = self->process_node->next;
                        self->process_index = 0;
                        ret = true;
                }
        } else {
                USH_ASSERT(false);
        }

        return ret;
}

#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
