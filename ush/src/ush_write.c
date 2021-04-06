#include "ush_internal.h"
#include "ush_config.h"

#include <string.h>

void ush_write_copy(struct ush_object *self, char *text, ush_state_t write_next_state)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(text != NULL);
        USH_ASSERT(write_next_state < USH_STATE__TOTAL_NUM);
        
        strncpy(self->desc->output_buffer, text, self->desc->output_buffer_size);
        size_t text_length = strnlen(self->desc->output_buffer, self->desc->output_buffer_size);

        self->write_pos = 0;
        self->write_size = text_length;
        self->write_buf = self->desc->output_buffer;

        self->state = USH_STATE_WRITE_CHAR;
        self->write_next_state = write_next_state;
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t write_next_state)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(text != NULL);
        USH_ASSERT(write_next_state < USH_STATE__TOTAL_NUM);
        
        self->write_pos = 0;
        self->write_size = strlen(text);
        self->write_buf = text;

        self->state = USH_STATE_WRITE_CHAR;
        self->write_next_state = write_next_state;
}

void ush_write_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        if (self->write_pos >= self->write_size) {
                self->state = self->write_next_state;
                return;
        }

        char ch = self->write_buf[self->write_pos];

        if (self->desc->io->write(self, ch) == 0)
                return;
        
        self->write_pos++;
}
