#include "ush_internal.h"
#include "ush_config.h"
#include "ush_utils.h"
#include "ush_node.h"

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
        char abs_path[USH_CONFIG_PATH_MAX_LENGTH];

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

#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
