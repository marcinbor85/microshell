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

#include "shell.h"

#include <microshell.h>

#include <string.h>

#include "interface.h"
#include "commands.h"
#include "fs/fs.h"

#define SHELL_WORK_BUFFER_SIZE  256
#define SHELL_HOSTNAME_SIZE     16

static char g_hostname_data[SHELL_HOSTNAME_SIZE + 1];

static char g_input_buffer[SHELL_WORK_BUFFER_SIZE];
static char g_output_buffer[SHELL_WORK_BUFFER_SIZE];

static const struct ush_descriptor g_ush_desc = {
        .io = &g_ush_io_interface,
        .input_buffer = g_input_buffer,
        .input_buffer_size = sizeof(g_input_buffer),
        .output_buffer = g_output_buffer,
        .output_buffer_size = sizeof(g_output_buffer),
        .path_max_length = SHELL_WORK_BUFFER_SIZE,
        .hostname = g_hostname_data,
};

static struct ush_object g_ush;

void shell_init(void)
{
        strcpy(g_hostname_data, "host");

        ush_init(&g_ush, &g_ush_desc);

        commands_register(&g_ush);
        fs_mount(&g_ush);
}

char* shell_get_hostname(void)
{
        return g_hostname_data;
}

void shell_set_hostname(const char *hostname)
{
        strncpy(g_hostname_data, hostname, sizeof(g_hostname_data));
        g_hostname_data[sizeof(g_hostname_data) - 1] = 0;
}

bool shell_service(void)
{
        return ush_service(&g_ush);
}
