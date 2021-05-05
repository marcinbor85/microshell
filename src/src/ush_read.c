#include "inc/ush_internal.h"

bool ush_read_service(struct ush_object *self, bool *read)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_READ_PREPARE:
                ush_read_start(self);
                break;
        case USH_STATE_READ_CHAR:
                *read = ush_read_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
