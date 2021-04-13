#include "ush_internal.h"
#include "ush_config.h"
#include "ush_file.h"
#include "ush_utils.h"
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

int ush_parse_get_args(struct ush_object *self, char* *argv)
{
        char *ptr = self->desc->input_buffer;
        int argc = self->args_count;

        if (argv == NULL)
                return argc;

        for (int i = 0; i < argc; i++) {
                argv[i] = ptr;
                ptr += strlen(argv[i]) + 1;
        }

        return argc;        
}

void ush_parse_finish(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char *argv[self->args_count];
        int argc;

        argc = ush_parse_get_args(self, argv);
        if (argc == 0)
                return;
        
        if (self->desc->exec != NULL)
                self->desc->exec(self, NULL, argc, argv);

        struct ush_file_descriptor const *file = ush_file_find_by_name(self, argv[0]);
        if (file == NULL) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR);
                return;
        }

        if (file->exec == NULL) {
                ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_EXECUTABLE);
                return;
        }
        
        file->exec(self, file, argc, argv);
}

void ush_parse_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch = self->desc->input_buffer[self->in_pos++];

        if (ch == '\n' || ch == '\r') {
                self->desc->input_buffer[self->out_pos++] = '\0';
                self->state = USH_STATE_RESET_PROMPT;
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
                        self->state = USH_STATE_PARSE_STANDARD_ARG;
                        break;
                default:
                        self->desc->input_buffer[self->out_pos++] = ch;
                        self->args_count++;
                        self->state = USH_STATE_PARSE_STANDARD_ARG;
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
                        self->state = USH_STATE_PARSE_STANDARD_ARG;
                        break;
                case '\\':
                        self->escape_flag = true;
                        break;
                default:
                        self->desc->input_buffer[self->out_pos++] = ch;
                        break;
                }
                break;
        case USH_STATE_PARSE_STANDARD_ARG:
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
        case USH_STATE_PARSE_STANDARD_ARG:
        case USH_STATE_PARSE_SEARCH_STOP:
                ush_parse_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
