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

#include "etc.h"
#include "shell.h"

#include <string.h>

static const char *g_config_data =
        "# system configuration\r\n"
        "freq = 16MHz\r\n"
        "ram = 4kB\r\n";

static size_t g_config_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        *data = (uint8_t*)g_config_data;
        return strlen(g_config_data);
}

static size_t g_hostname_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        *data = (uint8_t*)shell_get_hostname();
        return strlen((char*)*data);
}

static void g_hostname_data_setter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        (void)self;
        (void)file;
        (void)size;

        shell_set_hostname((char*)data);
}

static const struct ush_file_descriptor g_etc_desc[] = {
        {
                .name = "config",
                .description = "system configuration",
                .get_data = g_config_data_getter,
        },
        {
                .name = "hostname",
                .description = "shell hostname",
                .get_data = g_hostname_data_getter,
                .set_data = g_hostname_data_setter,
        },
};

static struct ush_node_object g_etc;

void etc_mount(struct ush_object *ush)
{
        ush_node_mount(ush, "/etc", &g_etc, g_etc_desc, sizeof(g_etc_desc) / sizeof(g_etc_desc[0]));
}
