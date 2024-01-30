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
#include "inc/ush_file.h"
#include "inc/ush.h"

#include <string.h>

void ush_parse_finish(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char *argv[self->args_count + 1];
        int argc;

        argc = ush_parse_get_args(self, argv);
        if (argc == 0)
                return;

        /* C standard says 'argv[argc] shall be a null pointer' */
        argv[argc] = NULL;

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
                ush_parse_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
