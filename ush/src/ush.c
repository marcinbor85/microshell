#include "ush.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(desc != NULL);

        USH_ASSERT(desc->cmd_buffer != NULL);
        USH_ASSERT(desc->cmd_buffer_size > 0);
        USH_ASSERT(desc->iface != NULL);

        USH_ASSERT(desc->iface->read != NULL);
        USH_ASSERT(desc->iface->write != NULL);

        self->desc = desc;
}

bool ush_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        (void)self;
        
        return false;
}
