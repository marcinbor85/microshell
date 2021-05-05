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

#include <microshell.h>

// non-blocking read interface
static int ush_read(struct ush_object *self, char *ch)
{
    *ch = Serial.read();
    return (*ch >= 0);
}

// non-blocking write interface
int ush_write(struct ush_object *self, char ch)
{
    return (Serial.write(ch) == 1);
}

// I/O interface descriptor
static const struct ush_io_interface ush_iface = {
    .read = ush_read,
    .write = ush_write,
};

// working buffers allocations
static char ush_in_buf[32];
static char ush_out_buf[32];

// microshell instance handler
static struct ush_object ush;

// microshell descriptor
static const struct ush_descriptor ush_desc = {
    .io = &ush_iface,                           // I/O interface pointer
    .input_buffer = ush_in_buf,                 // working input buffer
    .input_buffer_size = sizeof(ush_in_buf),    // working input buffer size
    .output_buffer = ush_out_buf,               // working output buffer
    .output_buffer_size = sizeof(ush_out_buf),  // working output buffer size
    .hostname = "arduino",                      // hostname (in prompt)
};

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

// info file get data callback
size_t info_get_data_callback(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
    static const char *info = "Use MicroShell and make fun!\r\n";

    // return pointer to data
    *data = info;
    // return data size
    return strlen(info);
}

// root directory files descriptor
static const struct ush_file_descriptor root_files[] = {
    {
        .name = "toggle",                   // toogle file name
        .description = "toggle led",        // optional file description
        .help = "usage: toggle\r\n",        // optional help manual
        .exec = toggle_exec_callback,       // optional execute callback
    },
    {
        .name = "set",                      // set file name
        .description = "set led",           // optional file description
        .help = "usage: set {0,1}\r\n",     // optional help manual
        .exec = set_exec_callback,          // optional execute callback
    },
    {
        .name = "info.txt",                 // info file name
        .description = NULL,
        .help = NULL,
        .exec = NULL,
        .get_data = info_get_data_callback, // optional data getter callback
    }
};

// root directory handler
static struct ush_node_object root;

void setup()
{
    // initialize I/O interface
    Serial.begin(9600);

    // initialize other hardware
    pinMode(LED_BUILTIN, OUTPUT);

    // initialize microshell instance
    ush_init(&ush, &ush_desc);

    // mount root directory
    ush_node_mount(&ush, "/", &root, root_files, sizeof(root_files) / sizeof(root_files[0]));
}

void loop()
{
    // non-blocking microshell service
    ush_service(&ush);

    // do other non-blocking stuff here
    // ...
}
