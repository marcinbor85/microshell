#include "ush_internal.h"
#include "ush_config.h"
#include "ush_shell.h"

#include <string.h>

void ush_prompt_write(struct ush_object *self, char *buf, size_t buf_size)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(buf != NULL);
        USH_ASSERT(buf_size > 0);
        
        snprintf(buf, buf_size, USH_SHELL_FONT_STYLE_BOLD USH_SHELL_FONT_COLOR_MAGENTA "[%s %s]$ " USH_SHELL_FONT_STYLE_RESET, self->desc->hostname, self->current_dir);
}

void ush_prompt_update(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        
        char prompt[USH_CONFIG_PROMPT_MAX_SIZE];

        ush_prompt_write(self, prompt, sizeof(prompt));
        ush_write_text(self, prompt, USH_STATE_READ_PREPARE);
}
