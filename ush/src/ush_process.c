#include "ush_internal.h"
#include "ush_types.h"
#include "ush_config.h"

void ush_process_start(struct ush_object *self, const struct ush_file_descriptor *file)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(file != NULL);
        
        self->process_file = file;
        self->state = USH_STATE_PROCESS_START;
}

bool ush_process_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PROCESS_START:
        case USH_STATE_PROCESS_SERVICE:
        case USH_STATE_PROCESS_FINISH:
                if (self->process_file->process == NULL) {
                        USH_ASSERT(false);
                        self->state = USH_STATE_RESET;
                        break;
                }
                self->process_file->process(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
