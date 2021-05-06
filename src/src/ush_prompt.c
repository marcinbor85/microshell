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

#include "inc/ush.h"
#include "inc/ush_shell.h"
#include "inc/ush_utils.h"
#include "inc/ush_internal.h"

#include <string.h>

void ush_prompt_start(struct ush_object *self, ush_state_t prompt_next_state)
{
        self->state = USH_STATE_PROMPT_PREFIX;
        self->prompt_next_state = prompt_next_state;
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
                char *path = ush_utils_path_last(self->current_node->path);
                ush_write_pointer(self, (char*)path, USH_STATE_PROMPT_SUFFIX);
                break;
        }
        case USH_STATE_PROMPT_SUFFIX:
                ush_write_pointer(self, "]$ " USH_SHELL_FONT_STYLE_RESET, self->prompt_next_state);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
