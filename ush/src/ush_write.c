#include "ush_internal.h"
#include "ush_config.h"

#include <string.h>

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


bool ush_write_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_WRITE_CHAR:
                ush_write_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
