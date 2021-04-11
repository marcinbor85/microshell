#include "ush_internal.h"
#include "ush_config.h"

void ush_autocomp_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        ush_write_pointer(self, "\r\n", USH_STATE_AUTOCOMP_PREPARE);
}

bool ush_autocomp_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_AUTOCOMP_PREPARE:
                /* TODO */
                ush_write_pointer(self, "TEST\r\n", USH_STATE_AUTOCOMP_PROMPT);
                break;
        case USH_STATE_AUTOCOMP_PROMPT:
                ush_prompt_start(self, USH_STATE_AUTOCOMP_RECALL);
                break;
        case USH_STATE_AUTOCOMP_RECALL:
                self->desc->input_buffer[self->in_pos] = '\0';
                ush_write_pointer(self, self->desc->input_buffer, USH_STATE_READ_CHAR);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
