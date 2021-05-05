#include "inc/ush_internal.h"
#include "inc/ush_preconfig.h"

void ush_write_pointer_bin(struct ush_object *self, uint8_t *data, size_t data_size, ush_state_t write_next_state)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(data != NULL);
        USH_ASSERT(write_next_state < USH_STATE__TOTAL_NUM);
        
        self->write_pos = 0;
        self->write_size = data_size;
        self->write_buf = (char*)data;

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
