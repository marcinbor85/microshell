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

// toggle file execute callback
static void toggle_exec_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
    // simple toggle led, without any arguments validation
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

// set file execute callback
static void set_exec_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
    // arguments count validation
    if (argc != 2) {
        // return predefined error message
        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
        return;
    }

    // arguments validation
    if (strcmp(argv[1], "1") == 0) {
        // turn led on
        digitalWrite(LED_BUILTIN, HIGH);
    } else if (strcmp(argv[1], "0") == 0) {
        // turn led off
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        // return predefined error message
        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
        return;
    }
}

// bin directory files descriptor
static const struct ush_file_descriptor bin_files[] = {
    {
        .name = "toggle",                       // toggle file name
        .description = "toggle led",            // optional file description
        .help = "usage: toggle\r\n",            // optional help manual
        .exec = toggle_exec_callback,           // optional execute callback
    },
    {
        .name = "set",                          // set file name
        .description = "set led",
        .help = "usage: set {0,1}\r\n",
        .exec = set_exec_callback
    },
};

// bin directory handler
static struct ush_node_object bin;

extern struct ush_object ush;

void shell_bin_mount(void)
{
    // mount bin directory
    ush_node_mount(&ush, "/bin", &bin, bin_files, sizeof(bin_files) / sizeof(bin_files[0]));
}
