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

void ush_write_pointer_bin(struct ush_object *self, uint8_t *data, size_t data_size, ush_state_t write_next_state)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(data != NULL);
        USH_ASSERT(write_next_state < USH_STATE__TOTAL_NUM);
        
        self->write_pos = 0;
        self->write_size = data_size;
        self->write_buf = (char*)data;

        self->state = USH_STATE_WRITE_CHAR;
        self->write_next_state = write_next_state;
}

void ush_write_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        if (self->write_pos >= self->write_size) {
                self->state = self->write_next_state;
                return;
        }

        char ch = self->write_buf[self->write_pos];

        if (self->desc->io->write(self, ch) == 0)
                return;
        
        self->write_pos++;
}
