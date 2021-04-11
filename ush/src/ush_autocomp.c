#include "ush_internal.h"
#include "ush_config.h"
#include "ush_utils.h"
#include "ush_node.h"

#include <string.h>

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
        self->desc->input_buffer[self->in_pos++] = self->autocomp_candidate_name[pos];
        if (self->in_pos >= self->desc->input_buffer_size)
                self->in_pos = 0;
        self->desc->input_buffer[self->in_pos] = '\0';
        self->autocomp_suffix_len++;

        ush_autocomp_prepare_candidates(self);
        self->state = USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE;
}

bool ush_autocomp_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {

        case USH_STATE_AUTOCOMP_PREPARE:
                self->autocomp_input = ush_utils_get_last_arg(self->desc->input_buffer);
                if (self->autocomp_input == NULL) {
                        self->state = USH_STATE_AUTOCOMP_PROMPT_PREPARE;
                } else {
                        self->state = USH_STATE_AUTOCOMP_CANDIDATES_START;

                }
                break;

        case USH_STATE_AUTOCOMP_CANDIDATES_START:
                self->autocomp_prev_count = 0;
                self->autocomp_suffix_len = 0;
                ush_autocomp_prepare_candidates(self);
                self->state = USH_STATE_AUTOCOMP_CANDIDATES_COUNT;
                break;

        case USH_STATE_AUTOCOMP_CANDIDATES_COUNT:
        case USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE:
        case USH_STATE_AUTOCOMP_CANDIDATES_PRINT: {
                if (self->process_node == NULL) {
                        if (self->process_stage == 0) {
                                self->process_node = self->current_node;
                                self->process_index = 0;
                                self->process_index_item = 0;
                                self->process_stage = 1;
                        } else if (self->process_stage == 1) {
                                char abs_path[USH_CONFIG_PATH_MAX_LENGTH];

                                ush_node_get_absolute_path(self, self->autocomp_input, abs_path);
                                if (self->autocomp_input[0] == '\0') {
                                        self->process_node = self->current_node;
                                } else {
                                        self->process_node = ush_node_get_parent_by_path(self, abs_path);
                                        if (self->process_node == NULL)
                                                self->process_node = self->current_node;
                                }

                                self->process_node = self->process_node->childs;
                                self->process_index = 0;
                                self->process_index_item = 0;
                                self->process_stage = 2;
                        } else if (self->process_stage == 2) {
                                self->autocomp_prev_state = self->state;
                                self->state = USH_STATE_AUTOCOMP_CANDIDATES_FINISH;
                        } else {
                                USH_ASSERT(false);
                        }
                        break;                        
                }

                if (self->process_stage == 0) {
                        if (self->process_index_item >= self->process_node->file_list_size) {
                                self->process_index_item = 0;
                                self->process_node = self->process_node->next;
                                break;
                        }
                } else if (self->process_stage == 1) {
                        if (self->process_index_item >= self->process_node->file_list_size) {
                                self->process_index_item = 0;
                                self->process_node = NULL;
                                break;
                        }
                } else if (self->process_stage == 2) {
                        /* do nothing */
                } else {
                        USH_ASSERT(false);
                }
                
                struct ush_file_descriptor const *file = NULL;
                
                if ((self->process_stage == 0) || (self->process_stage == 1)) {
                        file = &self->process_node->file_list[self->process_index_item];
                        if (ush_utils_startswith((char*)file->name, self->autocomp_input) == false) {
                                self->process_index_item++;
                                self->process_index = 0;
                                break;
                        }
                } else if (self->process_stage == 2) {
                        if (ush_utils_startswith((char*)self->process_node->path, self->autocomp_input) == false) {
                                self->process_node = self->process_node->next;
                                self->process_index = 0;
                                break;
                        }
                } else {
                        USH_ASSERT(false);
                }

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
                                        ush_write_pointer(self, (char*)self->process_node->path, self->state);
                                self->process_index = 1;
                                self->autocomp_count++;
                                self->autocomp_candidate_name = (char*)self->process_node->path;
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
                break;
        }

        case USH_STATE_AUTOCOMP_CANDIDATES_FINISH:

                if (self->autocomp_prev_state == USH_STATE_AUTOCOMP_CANDIDATES_PRINT) {
                        self->state = USH_STATE_AUTOCOMP_PROMPT;
                        break;
                }

                if (self->autocomp_prev_state == USH_STATE_AUTOCOMP_CANDIDATES_COUNT) {
                        switch (self->autocomp_count) {
                        case 0:
                                self->state = USH_STATE_READ_CHAR;
                                break;
                        case 1: {
                                char *suffix = self->autocomp_input + strlen(self->autocomp_input);
                                strcpy(self->autocomp_input, self->autocomp_candidate_name);
                                self->in_pos = strlen(self->desc->input_buffer);
                                ush_write_pointer(self, suffix, USH_STATE_READ_CHAR);
                                break;
                        }
                        default:
                                ush_autocomp_optimize_continue(self);
                                break;
                        }

                } else if (self->autocomp_prev_state == USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE) {
                        if (self->autocomp_count < self->autocomp_prev_count) {
                                if (self->in_pos > 0)
                                        self->in_pos--;
                                self->desc->input_buffer[self->in_pos] = '\0';
                                self->autocomp_suffix_len--;

                                ush_autocomp_prepare_candidates(self);

                                char *suffix = self->autocomp_input + strlen(self->autocomp_input) - self->autocomp_suffix_len;
                                if (strlen(suffix) > 0) {
                                        ush_write_pointer(self, suffix, USH_STATE_READ_CHAR);
                                } else {
                                        ush_autocomp_prepare_candidates(self);
                                        ush_write_pointer(self, "\r\n", USH_STATE_AUTOCOMP_CANDIDATES_PRINT);
                                }
                                break;
                        }

                        ush_autocomp_optimize_continue(self);
                } else {
                        USH_ASSERT(false);
                }
                break;

        case USH_STATE_AUTOCOMP_PROMPT_PREPARE:
                ush_write_pointer(self, "\r\n", USH_STATE_AUTOCOMP_PROMPT);
                break;

        case USH_STATE_AUTOCOMP_PROMPT:
                ush_prompt_start(self, USH_STATE_AUTOCOMP_RECALL);
                break;

        case USH_STATE_AUTOCOMP_RECALL:
                ush_write_pointer(self, self->desc->input_buffer, USH_STATE_READ_CHAR);
                break;

        default:
                processed = false;
                break;

        }

        return processed;
}
