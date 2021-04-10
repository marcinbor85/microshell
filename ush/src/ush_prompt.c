#include "ush_internal.h"
#include "ush_shell.h"
#include "ush.h"

#include <string.h>

void ush_prompt_start(struct ush_object *self)
{
        self->state = USH_STATE_PROMPT_PREFIX;
}

bool ush_prompt_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PROMPT_PREFIX:
                ush_write_pointer(self, USH_SHELL_FONT_COLOR_MAGENTA "[", USH_STATE_PROMPT_HOST);
                break;
        case USH_STATE_PROMPT_HOST:
                ush_write_pointer(self, self->desc->hostname, USH_STATE_PROMPT_SPACE);
                break;
        case USH_STATE_PROMPT_SPACE:
                ush_write_pointer(self, " ", USH_STATE_PROMPT_PATH);
                break;
        case USH_STATE_PROMPT_PATH: {
                char *path = NULL;
                ush_utils_path_last(self->current_node->path, &path);
                ush_write_pointer(self, (char*)path, USH_STATE_PROMPT_SUFFIX);
                break;
        }
        case USH_STATE_PROMPT_SUFFIX:
                ush_write_pointer(self, "]$ " USH_SHELL_FONT_STYLE_RESET, USH_STATE_READ_PREPARE);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
