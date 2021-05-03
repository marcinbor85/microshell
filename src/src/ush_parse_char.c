#include "ush_internal.h"
#include "ush_config.h"

void ush_parse_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch = self->desc->input_buffer[self->in_pos++];

        if (ch == '\n' || ch == '\r') {
                self->desc->input_buffer[self->out_pos++] = '\0';
                self->state = USH_STATE_RESET_PROMPT;
                ush_parse_finish(self);
                return;
        }

        ush_parse_char_standard(self, ch);
}
