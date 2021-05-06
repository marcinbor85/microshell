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

#include "interface.h"
#include "terminal.h"

#include <stdio.h>

static FILE *g_io_read;
static FILE *g_io_write;
static const char *g_io_filename;

static int write_char(struct ush_object *self, char ch)
{
        (void)self;

        char c = fputc(ch, g_io_write);
        if (c != ch)
                return 0;
        return 1;
}

static int read_char(struct ush_object *self, char *ch)
{
        (void)self;
     
        *ch = fgetc(g_io_read);
        return 1;
}

const struct ush_io_interface g_ush_io_interface = {
        .read = read_char,
        .write = write_char,
};

void interface_open(const char *filename)
{
        g_io_filename = filename;

        if (filename == NULL) {
                g_io_read = stdin;
                g_io_write = stdout;
                terminal_set_input_mode();
        } else {
                g_io_read = fopen(filename, "a+");
                if (g_io_read == NULL) {
                        fprintf(stderr, "cannot open device\n");
                        exit(EXIT_FAILURE);
                }
                g_io_write = g_io_read;
        }
}

void interface_close(void)
{
        if (g_io_filename != NULL) {
                fclose(g_io_read);
                fclose(g_io_write);
        }
        terminal_reset_input_mode();
}
