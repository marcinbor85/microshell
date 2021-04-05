#include "ush_internal.h"
#include "ush_config.h"

#include <string.h>

bool ush_write_is_ready(struct ush_object *self)
{
        if (self->write.state != USH_WRITE_STATE_IDLE)
                return false;
        
        if (self->write.mutex != false)
                return false;
        
        return true;
}

void ush_write_text(struct ush_object *self, char *text)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(text != NULL);
        
        strncpy(self->desc->output_buffer, text, self->desc->output_buffer_size);
        self->write.size = strnlen(self->desc->output_buffer, self->desc->output_buffer_size);
        self->write.position = 0;
        self->write.state = USH_WRITE_STATE_PROCESS;
}

void ush_write_reset(struct ush_object *self)
{
        self->write.state = USH_WRITE_STATE_IDLE;
}

bool ush_write_take_buffer(struct ush_object *self)
{
        if (self->write.mutex == false) {
                self->write.mutex = true;
                return true;
        }
        return false;
}

void ush_write_give_buffer(struct ush_object *self)
{
        self->write.mutex = false;
}

void ush_write_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        if (self->write.position >= self->write.size) {
                self->write.state = USH_WRITE_STATE_IDLE;
                return;
        }

        char ch = self->desc->output_buffer[self->write.position];

        if (self->desc->iface->write(self, ch) == 0)
                return;
        
        self->write.position++;
}

bool ush_write_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        
        bool busy = false;

        switch (self->write.state) {
        case USH_WRITE_STATE_IDLE:
                break;
        case USH_WRITE_STATE_PROCESS:
                ush_write_char(self);
                busy = true;
                break;
        default:
                USH_ASSERT(false);
                break;
        }

        return busy;
}
