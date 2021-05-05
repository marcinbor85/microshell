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

// reboot cmd file execute callback
static void reboot_exec_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
#if defined(ARDUINO_ARCH_ESP32)
    ESP.restart();
#elif defined(ARDUINO_ARCH_AVR)
    void (*reset)(void) = 0;
    reset();
#else
    ush_print(self, "error: reboot not supported...");
#endif
}

// cmd files descriptor
static const struct ush_file_descriptor cmd_files[] = {
    {
        .name = "reboot",
        .description = "reboot device",
        .help = NULL,
        .exec = reboot_exec_callback,
    },
};

// cmd commands handler
static struct ush_node_object cmd;

extern struct ush_object ush;

void shell_commands_add(void)
{
    // add custom commands
    ush_commands_add(&ush, &cmd, cmd_files, sizeof(cmd_files) / sizeof(cmd_files[0]));
}
