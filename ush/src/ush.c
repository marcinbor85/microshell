#include "ush.h"
#include "ush_internal.h"
#include "ush_shell.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(desc != NULL);

        USH_ASSERT(desc->input_buffer != NULL);
        USH_ASSERT(desc->input_buffer_size > 0);
        USH_ASSERT(desc->output_buffer != NULL);
        USH_ASSERT(desc->output_buffer_size > 0);

        USH_ASSERT(desc->io != NULL);
        USH_ASSERT(desc->io->read != NULL);
        USH_ASSERT(desc->io->write != NULL);

        USH_ASSERT(desc->hostname != NULL);

        self->desc = desc;

        snprintf(self->current_dir, sizeof(self->current_dir), "/");

        ush_reset(self);
}

bool ush_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool busy = false;

        switch (self->state) {
        case USH_STATE_RESET:
                self->state = USH_STATE_PROMPT_PREFIX;
                busy = true;
                break;
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
        case USH_STATE_READ_PREPARE:
                ush_read_start(self);
                busy = true;
                break;
        case USH_STATE_READ_CHAR:
                busy = ush_read_char(self);
                break;
        case USH_STATE_PARSE_PREPARE:
                ush_parse_start(self);
                busy = true;
                break;
        case USH_STATE_PARSE_SEARCH_ARG:
        case USH_STATE_PARSE_QUOTE_ARG:
        case USG_STATE_PARSE_STANDARD_ARG:
        case USH_STATE_PARSE_SEARCH_STOP:
                ush_parse_char(self);
                busy = true;
                break;
        case USH_STATE_WRITE_CHAR:
                ush_write_char(self);
                busy = true;
                break;
        default:
                USH_ASSERT(false);
                break;
        }

        return busy;
}

void ush_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->state = USH_STATE_RESET;
}
