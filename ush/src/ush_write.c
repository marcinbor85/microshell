#include "ush_internal.h"
#include "ush_config.h"

#include <string.h>

void ush_write_text(struct ush_object *self, char *text, ush_state_t next_write_state)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(text != NULL);
        
        strncpy(self->desc->output_buffer, text, self->desc->output_buffer_size);
        self->output_size = strnlen(self->desc->output_buffer, self->desc->output_buffer_size);
        self->output_position = 0;
        self->state = USH_STATE_WRITE_CHAR;
        self->next_write_state = next_write_state;
}

void ush_write_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        if (self->output_position >= self->output_size) {
                self->state = self->next_write_state;
                return;
        }

        char ch = self->desc->output_buffer[self->output_position];

        if (self->desc->iface->write(self, ch) == 0)
                return;
        
        self->output_position++;
}
