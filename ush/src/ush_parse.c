#include "ush_internal.h"
#include "ush_config.h"
#include "ush.h"

#include <string.h>

void ush_parse_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->in_pos = 0;
        self->out_pos = 0;
        self->args_count = 0;
        self->escape_flag = false;
        self->state = USH_STATE_PARSE_SEARCH_ARG;
}

void ush_parse_finish(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        int i;
        char *argv[self->args_count];
        int argc = self->args_count;
        char *ptr = self->desc->input_buffer;
        char* buf;

        for (i = 0; i < argc; i++) {
                argv[i] = ptr;
                ptr += strlen(argv[i]) + 1;
        }

        if (self->desc->cmd_callback != NULL) {
                buf = self->desc->cmd_callback(self, argc, argv);
                if (buf != NULL) {
                        ush_write_pointer(self, buf, USH_STATE_RESET);
                        return;
                }
        } else {
                if (argc > 0) {
                        struct ush_cmd_object *cmd = ush_cmd_find_by_name(self, argv[0]);
                        if (cmd != NULL) {
                                if (cmd->desc->cmd_callback != NULL) {
                                        buf = cmd->desc->cmd_callback(self, cmd, argc, argv);
                                        if (buf != NULL) {
                                                ush_write_pointer(self, buf, USH_STATE_RESET);
                                                return;
                                        }
                                }
                        } else {
                                ush_write_pointer(self, "unknown command\r\n", USH_STATE_RESET);
                                return;
                        }
                }
        }
        self->state = USH_STATE_RESET;
}

void ush_parse_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch = self->desc->input_buffer[self->in_pos++];

        if (ch == '\n' || ch == '\r') {
                self->desc->input_buffer[self->out_pos++] = '\0';
                ush_parse_finish(self);
                return;
        }

        switch (self->state) {
        case USH_STATE_PARSE_SEARCH_ARG:
                switch (ch) {
                case ' ':
                        break;
                case '\"':
                        self->args_count++;
                        self->state = USH_STATE_PARSE_QUOTE_ARG;
                        break;
                case '\\':
                        self->escape_flag = true;
                        self->args_count++;
                        self->state = USG_STATE_PARSE_STANDARD_ARG;
                        break;
                default:
                        self->desc->input_buffer[self->out_pos++] = ch;
                        self->args_count++;
                        self->state = USG_STATE_PARSE_STANDARD_ARG;
                        break;
                }
                break;
        case USH_STATE_PARSE_QUOTE_ARG:
                if (self->escape_flag != false) {
                        self->desc->input_buffer[self->out_pos++] = ch;
                        self->escape_flag = false;
                        break;
                }

                switch (ch) {
                case '\"':
                        self->state = USG_STATE_PARSE_STANDARD_ARG;
                        break;
                case '\\':
                        self->escape_flag = true;
                        break;
                default:
                        self->desc->input_buffer[self->out_pos++] = ch;
                        break;
                }
                break;
        case USG_STATE_PARSE_STANDARD_ARG:
                if (self->escape_flag != false) {
                        self->desc->input_buffer[self->out_pos++] = ch;
                        self->escape_flag = false;
                        break;
                }

                switch (ch) {
                case ' ':
                        self->desc->input_buffer[self->out_pos++] = '\0';
                        self->state = USH_STATE_PARSE_SEARCH_ARG;
                        break;
                case '\"':
                        self->state = USH_STATE_PARSE_QUOTE_ARG;
                        break;
                case '\\':
                        self->escape_flag = true;
                        break;
                default:
                        self->desc->input_buffer[self->out_pos++] = ch;
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

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PARSE_PREPARE:
                ush_parse_start(self);
                break;
        case USH_STATE_PARSE_SEARCH_ARG:
        case USH_STATE_PARSE_QUOTE_ARG:
        case USG_STATE_PARSE_STANDARD_ARG:
        case USH_STATE_PARSE_SEARCH_STOP:
                ush_parse_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
