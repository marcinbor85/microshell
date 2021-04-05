#include "ush_internal.h"
#include "ush_config.h"
#include "ush.h"

#include <string.h>

void ush_parse_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->parse.input_position = 0;
        self->parse.output_position = 0;
        self->parse.argc = 0;
        self->parse.escape = false;
        self->parse.state = USH_PARSE_STATE_WAIT_BUFFER;
}

void ush_parse_finish(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        int i;
        char *argv[self->parse.argc];
        int argc = self->parse.argc;
        char *ptr = self->desc->output_buffer;

        for (i = 0; i < argc; i++) {
                argv[i] = ptr;
                ptr += strlen(argv[i]) + 1;
        }

        char buf[128];
        memset(buf, 0, sizeof(buf));
        for (i = 0; i < argc; i++) {
                strcat(buf, argv[i]);
                strcat(buf, " ");
        }
        ush_write_text(self, buf);

        /* TODO: callback */
}

bool ush_parse_is_ready(struct ush_object *self)
{
        return (self->parse.state == USH_PARSE_STATE_IDLE) ? true : false;
}

void ush_parse_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->parse.state = USH_PARSE_STATE_IDLE;
}

void ush_parse_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch = self->desc->input_buffer[self->parse.input_position++];

        if (ch == '\n') {
                self->desc->output_buffer[self->parse.output_position++] = '\0';
                ush_write_give_buffer(self);
                ush_parse_finish(self);
                ush_read_start(self);
                self->parse.state = USH_PARSE_STATE_WAIT_PROCESS;
                return;
        }

        switch (self->parse.state) {
        case USH_PARSE_STATE_SEARCH_ARG:
                switch (ch) {
                case ' ':
                        break;
                case '\"':
                        self->parse.argc++;
                        self->parse.state = USH_PARSE_STATE_QUOTE_ARG;
                        break;
                case '\\':
                        self->parse.escape = true;
                        self->parse.argc++;
                        self->parse.state = USG_PARSE_STATE_STANDARD_ARG;
                        break;
                default:
                        self->desc->output_buffer[self->parse.output_position++] = ch;
                        self->parse.argc++;
                        self->parse.state = USG_PARSE_STATE_STANDARD_ARG;
                        break;
                }
                break;
        case USH_PARSE_STATE_QUOTE_ARG:
                if (self->parse.escape != false) {
                        self->desc->output_buffer[self->parse.output_position++] = ch;
                        self->parse.escape = false;
                        break;
                }

                switch (ch) {
                case '\"':
                        self->desc->output_buffer[self->parse.output_position++] = '\0';
                        self->parse.state = USH_PARSE_STATE_SEARCH_ARG;
                        break;
                case '\\':
                        self->parse.escape = true;
                        break;
                default:
                        self->desc->output_buffer[self->parse.output_position++] = ch;
                        break;
                }
                break;
        case USG_PARSE_STATE_STANDARD_ARG:
                if (self->parse.escape != false) {
                        self->desc->output_buffer[self->parse.output_position++] = ch;
                        self->parse.escape = false;
                        break;
                }

                switch (ch) {
                case ' ':
                        self->desc->output_buffer[self->parse.output_position++] = '\0';
                        self->parse.state = USH_PARSE_STATE_SEARCH_ARG;
                        break;
                case '\\':
                        self->parse.escape = true;
                        break;
                default:
                        self->desc->output_buffer[self->parse.output_position++] = ch;
                        break;
                }
                break;
        default:
                USH_ASSERT(false);
                break;
        }
}

bool ush_parse_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        
        bool busy = false;

        switch (self->parse.state) {
        case USH_PARSE_STATE_IDLE:
                break;
        case USH_PARSE_STATE_WAIT_BUFFER:
                if (ush_write_take_buffer(self) != false)
                        self->parse.state = USH_PARSE_STATE_SEARCH_ARG;
                break;
        case USH_PARSE_STATE_SEARCH_ARG:
        case USH_PARSE_STATE_QUOTE_ARG:
        case USG_PARSE_STATE_STANDARD_ARG:
        case USH_PARSE_STATE_SEARCH_STOP:
                ush_parse_char(self);
                busy = true;
                break;
        case USH_PARSE_STATE_WAIT_PROCESS:
                if (ush_write_is_ready(self) == false)
                        break;

                ush_prompt_update(self);
                ush_parse_reset(self);
                break;
        default:
                USH_ASSERT(false);
                break;
        }

        return busy;
}
