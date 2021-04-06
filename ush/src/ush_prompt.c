#include "ush_internal.h"
#include "ush_shell.h"
#include "ush.h"

#include <string.h>

bool ush_prompt_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PROMPT_PREFIX:
                ush_write_pointer(self, USH_SHELL_FONT_STYLE_BOLD USH_SHELL_FONT_COLOR_MAGENTA "[", USH_STATE_PROMPT_HOST);
                break;
        case USH_STATE_PROMPT_HOST:
                ush_write_pointer(self, self->desc->hostname, USH_STATE_PROMPT_SPACE);
                break;
        case USH_STATE_PROMPT_SPACE:
                ush_write_pointer(self, " ", USH_STATE_PROMPT_PATH);
                break;
        case USH_STATE_PROMPT_PATH:
                ush_write_pointer(self, self->current_dir, USH_STATE_PROMPT_SUFFIX);
                break;
        case USH_STATE_PROMPT_SUFFIX:
                ush_write_pointer(self, "]$ " USH_SHELL_FONT_STYLE_RESET, USH_STATE_READ_PREPARE);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
