#include "inc/ush_internal.h"
#include "inc/ush_preconfig.h"
#include "inc/ush_utils.h"
#include "inc/ush_node.h"

#include <string.h>

#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1

bool ush_autocomp_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {

        case USH_STATE_AUTOCOMP_PREPARE:
                ush_autocomp_state_prepare(self);
                break;

        case USH_STATE_AUTOCOMP_CANDIDATES_START:
                ush_autocomp_state_candidates_start(self);
                break;

        case USH_STATE_AUTOCOMP_CANDIDATES_COUNT:
        case USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE:
        case USH_STATE_AUTOCOMP_CANDIDATES_PRINT: {
                ush_autocomp_state_candidates_process(self);
                break;
        }

        case USH_STATE_AUTOCOMP_CANDIDATES_FINISH:
                ush_autocomp_state_candidates_finish(self);
                break;

        case USH_STATE_AUTOCOMP_PROMPT_PREPARE:
                ush_write_pointer(self, "\r\n", USH_STATE_AUTOCOMP_PROMPT);
                break;

        case USH_STATE_AUTOCOMP_PROMPT:
                ush_prompt_start(self, USH_STATE_AUTOCOMP_RECALL);
                break;

        case USH_STATE_AUTOCOMP_RECALL:
                ush_write_pointer(self, self->desc->input_buffer, USH_STATE_READ_CHAR);
                break;
        
        case USH_STATE_AUTOCOMP_RECALL_SUFFIX: {                
                ush_autocomp_state_recall_suffix(self);
                break;
        }

        default:
                processed = false;
                break;

        }

        return processed;
}

#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
