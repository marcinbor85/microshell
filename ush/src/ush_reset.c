#include "ush_internal.h"
#include "ush.h"

#include <string.h>

void ush_reset_start(struct ush_object *self)
{
        self->state = USH_STATE_RESET;
}

bool ush_reset_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_RESET:
                ush_prompt_start(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
