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

// non-blocking read interface
static int ush_read(struct ush_object *self, char *ch)
{
    // should be implemented as a FIFO
    if (Serial.available() > 0) {
        *ch = Serial.read();
        return 1;
    }
    return 0;
}

// non-blocking write interface
static int ush_write(struct ush_object *self, char ch)
{
    // should be implemented as a FIFO
    return (Serial.write(ch) == 1);
}

// I/O interface descriptor
static const struct ush_io_interface ush_iface = {
    .read = ush_read,
    .write = ush_write,
};

// working buffers allocations (size could be customized)
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_ARCH_STM32)
    #define BUF_IN_SIZE    128
    #define BUF_OUT_SIZE   128
    #define PATH_MAX_SIZE  128
#else
    #define BUF_IN_SIZE    32
    #define BUF_OUT_SIZE   32
    #define PATH_MAX_SIZE  32
#endif

static char ush_in_buf[BUF_IN_SIZE];
static char ush_out_buf[BUF_OUT_SIZE];

// microshell instance handler
struct ush_object ush;

// microshell descriptor
static const struct ush_descriptor ush_desc = {
    .io = &ush_iface,                           // I/O interface pointer
    .input_buffer = ush_in_buf,                 // working input buffer
    .input_buffer_size = sizeof(ush_in_buf),    // working input buffer size
    .output_buffer = ush_out_buf,               // working output buffer
    .output_buffer_size = sizeof(ush_out_buf),  // working output buffer size
    .path_max_length = PATH_MAX_SIZE,           // path maximum length (stack)
    .hostname = "arduino",                      // hostname (in prompt)
};

extern void shell_commands_add(void);
extern void shell_root_mount(void);
extern void shell_dev_mount(void);
extern void shell_bin_mount(void);

void shell_init(void)
{
    // initialize I/O interface
    Serial.begin(115200UL);

    // initialize microshell instance
    ush_init(&ush, &ush_desc);

    // add commands
    shell_commands_add();

    // mount nodes (root must be first)
    shell_root_mount();
    shell_dev_mount();
    shell_bin_mount();
}

void shell_service(void)
{
    ush_service(&ush);
}