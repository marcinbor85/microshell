/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "inc/ush_internal.h"
#include "inc/ush_preconfig.h"

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

void ush_parse_char_standard(struct ush_object *self, char ch)
{
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
                        if (ch == 'x')
                                self->desc->input_buffer[self->out_pos++] = '\\';
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
                        if (ch == 'x')
                                self->desc->input_buffer[self->out_pos++] = '\\';
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
