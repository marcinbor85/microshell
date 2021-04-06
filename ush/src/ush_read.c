#include "ush_internal.h"
#include "ush_config.h"

void ush_read_echo_service(struct ush_object *self, char ch)
{
        char echo_buf[3];
        echo_buf[0] = ch;
        echo_buf[1] = (ch == '\r') ? '\n' : '\0';
        echo_buf[2] = '\0';

        ush_state_t next = (ch == '\r') ? USH_STATE_PARSE_PREPARE : USH_STATE_READ_CHAR;
        ush_write_copy(self, echo_buf, next);
}

bool ush_read_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch;

        if (self->desc->io->read(self, &ch) == 0)
                return false;

        self->desc->input_buffer[self->in_pos++] = ch;
        if (self->in_pos >= self->desc->input_buffer_size)
                self->in_pos = 0;

        ush_read_echo_service(self, ch);
        
        return true;
}

void ush_read_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->in_pos = 0;
        self->state = USH_STATE_READ_CHAR;
}
