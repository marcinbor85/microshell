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

        char echo[3];
        echo[0] = ch;
        if (ch == '\r') {
                echo[1] = '\n';
                echo[2] = '\0';
        } else {
                echo[1] = '\0';
        }

        ush_state_t next = (ch == '\r') ? USH_STATE_PARSE_PREPARE : USH_STATE_READ_CHAR;
        ush_write_text(self, echo, next);
        
        return true;
}

void ush_read_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->input_position = 0;
        self->state = USH_STATE_READ_CHAR;
}
