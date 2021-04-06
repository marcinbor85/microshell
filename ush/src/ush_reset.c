#include "ush_internal.h"
#include "ush.h"

#include <string.h>

bool ush_reset_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_RESET:
                self->state = USH_STATE_PROMPT_PREFIX;
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
