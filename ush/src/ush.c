#include "ush.h"
#include "ush_internal.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(desc != NULL);

        USH_ASSERT(desc->input_buffer != NULL);
        USH_ASSERT(desc->input_buffer_size > 0);
        USH_ASSERT(desc->output_buffer != NULL);
        USH_ASSERT(desc->output_buffer_size > 0);

        USH_ASSERT(desc->iface != NULL);
        USH_ASSERT(desc->iface->read != NULL);
        USH_ASSERT(desc->iface->write != NULL);

        USH_ASSERT(desc->hostname != NULL);

        self->desc = desc;

        snprintf(self->current_dir, sizeof(self->current_dir), "/");

        ush_reset(self);
}

bool ush_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool busy = false;

        busy |= ush_read_service(self);
        busy |= ush_parse_service(self);
        busy |= ush_write_service(self);
        
        return busy;
}

void ush_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        ush_read_start(self);
        ush_parse_reset(self);
        ush_write_reset(self);

        ush_prompt_update(self);
}
