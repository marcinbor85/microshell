#include "ush_internal.h"
#include "ush_config.h"

#include <string.h>

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t write_next_state)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(text != NULL);
        USH_ASSERT(write_next_state < USH_STATE__TOTAL_NUM);
        
        ush_write_pointer_bin(self, (uint8_t*)text, strlen(text), write_next_state);
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
