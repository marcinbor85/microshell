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

#include <Arduino.h>
#include <microshell.h>

// info file get data callback
size_t info_get_data_callback(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
    static const char *info = "Use MicroShell and make fun!\r\n";

    // return pointer to data
    *data = (uint8_t*)info;
    // return data size
    return strlen(info);
}

// root directory files descriptor
static const struct ush_file_descriptor root_files[] = {
    {
        .name = "info.txt",                     // info.txt file name
        .description = NULL,
        .help = NULL,
        .exec = NULL,
        .get_data = info_get_data_callback,
    }
};

// root directory handler
static struct ush_node_object root;

extern struct ush_object ush;

void shell_root_mount(void)
{
    // mount root directory
    ush_node_mount(&ush, "/", &root, root_files, sizeof(root_files) / sizeof(root_files[0]));
}
