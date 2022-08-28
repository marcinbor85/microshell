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

#include "bin.h"

#include <time.h>

static time_t start_time;

static void print_exec_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        
        if (argc != 2) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        ush_print(self, argv[1]);
}

static void uptime_exec_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argv;

        if (argc != 1) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        ush_printf(self, "time: %lus\r\n", time(NULL) - start_time);
}

static const struct ush_file_descriptor g_bin_desc[] = {
        {
                .name = "uptime",
                .help = "usage: uptime\r\n  show system uptime\r\n",
                .exec = uptime_exec_callback,
        },
        {
                .name = "print",
                .help = "usage: print <arg>\r\n  print argument to shell\r\n",
                .exec = print_exec_callback,
        }
};


static struct ush_node_object g_bin;

void bin_mount(struct ush_object *ush)
{
        start_time = time(NULL);

        ush_node_mount(ush, "/bin", &g_bin, g_bin_desc, sizeof(g_bin_desc) / sizeof(g_bin_desc[0]));
}
