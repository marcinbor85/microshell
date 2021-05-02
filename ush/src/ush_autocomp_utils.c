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

#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
