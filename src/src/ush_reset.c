#include "ush_internal.h"
#include "ush_const.h"
#include "ush.h"

#include <string.h>

void ush_reset_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->state = USH_STATE_RESET;
}

void ush_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        
        self->current_node = self->root;
        ush_write_pointer(self, USH_NAME " " USH_VERSION "\r\n", USH_STATE_RESET_PROMPT);
}

bool ush_reset_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_RESET:
                ush_write_pointer(self, "\r\n", USH_STATE_RESET_PROMPT);
                break;
        case USH_STATE_RESET_PROMPT:
                ush_prompt_start(self, USH_STATE_READ_PREPARE);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
