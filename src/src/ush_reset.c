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
#include "inc/ush_const.h"
#include "inc/ush.h"

#include <string.h>

void ush_reset_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->state = USH_STATE_RESET;
}

void ush_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        
        self->current_node = self->root;
        ush_write_pointer(self, USH_NAME " " USH_VERSION "\r\n", USH_STATE_RESET_PROMPT);
}

bool ush_reset_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_RESET:
                ush_write_pointer(self, "\r\n", USH_STATE_RESET_PROMPT);
                break;
        case USH_STATE_RESET_PROMPT:
                ush_prompt_start(self, USH_STATE_READ_PREPARE);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
