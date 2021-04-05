#include "ush_internal.h"
#include "ush_config.h"

bool ush_read_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch;

        if (self->desc->iface->read(self, &ch) == 0)
                return false;
        
        if (self->input_position >= self->desc->input_buffer_size)
                self->input_position = 0;

        self->desc->input_buffer[self->input_position++] = ch;

        if (ch != '\n')
                return false;
        
        ush_parse_start(self);
        return true;
}

void ush_read_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->input_position = 0;
        self->state = USH_STATE_READ_CHAR;
}
