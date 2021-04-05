#include "ush_internal.h"
#include "ush_config.h"
#include "ush.h"

#include <string.h>

void ush_parse_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->input_position = 0;
        self->output_position = 0;
        self->argc = 0;
        self->escape = false;
        self->state = USH_STATE_PARSE_SEARCH_ARG;
}

void ush_parse_finish(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        int i;
        char *argv[self->argc];
        int argc = self->argc;
        char *ptr = self->desc->output_buffer;

        for (i = 0; i < argc; i++) {
                argv[i] = ptr;
                ptr += strlen(argv[i]) + 1;
        }

        char buf[128];
        memset(buf, 0, sizeof(buf));
        for (i = 0; i < argc; i++) {
                strcat(buf, argv[i]);
                strcat(buf, "\n");
        }
        ush_write_text(self, buf, USH_STATE_RESET);

        /* TODO: callback */
}

void ush_parse_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch = self->desc->input_buffer[self->input_position++];

        if (ch == '\n') {
                self->desc->output_buffer[self->output_position++] = '\0';
                ush_parse_finish(self);
                return;
        }

        switch (self->state) {
        case USH_STATE_PARSE_SEARCH_ARG:
                switch (ch) {
                case ' ':
                        break;
                case '\"':
                        self->argc++;
                        self->state = USH_STATE_PARSE_QUOTE_ARG;
                        break;
                case '\\':
                        self->escape = true;
                        self->argc++;
                        self->state = USG_STATE_PARSE_STANDARD_ARG;
                        break;
                default:
                        self->desc->output_buffer[self->output_position++] = ch;
                        self->argc++;
                        self->state = USG_STATE_PARSE_STANDARD_ARG;
                        break;
                }
                break;
        case USH_STATE_PARSE_QUOTE_ARG:
                if (self->escape != false) {
                        self->desc->output_buffer[self->output_position++] = ch;
                        self->escape = false;
                        break;
                }

                switch (ch) {
                case '\"':
                        self->state = USG_STATE_PARSE_STANDARD_ARG;
                        break;
                case '\\':
                        self->escape = true;
                        break;
                default:
                        self->desc->output_buffer[self->output_position++] = ch;
                        break;
                }
                break;
        case USG_STATE_PARSE_STANDARD_ARG:
                if (self->escape != false) {
                        self->desc->output_buffer[self->output_position++] = ch;
                        self->escape = false;
                        break;
                }

                switch (ch) {
                case ' ':
                        self->desc->output_buffer[self->output_position++] = '\0';
                        self->state = USH_STATE_PARSE_SEARCH_ARG;
                        break;
                case '\"':
                        self->state = USH_STATE_PARSE_QUOTE_ARG;
                        break;
                case '\\':
                        self->escape = true;
                        break;
                default:
                        self->desc->output_buffer[self->output_position++] = ch;
                        break;
                }
                break;
        default:
                USH_ASSERT(false);
                break;
        }
}
