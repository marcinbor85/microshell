#include "ush_internal.h"
#include "ush_config.h"

bool ush_read_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch;

        if (self->desc->iface->read(self, &ch) == 0)
                return false;
        
        if (self->read.input_buffer_index >= self->desc->input_buffer_size)
                self->read.input_buffer_index = 0;

        self->desc->input_buffer[self->read.input_buffer_index++] = ch;

        if (ch != '\n')
                return false;
        
        self->read.state = USH_READ_STATE_WAIT_PARSE;
        return true;
}

void ush_read_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->read.input_buffer_index = 0;
        self->read.state = USH_READ_STATE_READ;
}

bool ush_read_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        
        bool busy = false;

        switch (self->read.state) {
        case USH_READ_STATE_READ:
                busy = ush_read_char(self);
                break;
        case USH_READ_STATE_WAIT_PARSE:
                busy = true;
                if (ush_parse_is_ready(self) == false)
                        break;
                ush_parse_start(self);
                self->read.state = USH_READ_STATE_WAIT_PARSE_END;
                break;
        case USH_READ_STATE_WAIT_PARSE_END:
                break;
        default:
                USH_ASSERT(false);
                break;
        }

        return busy;
}
