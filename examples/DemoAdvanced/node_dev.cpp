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

// led file get data callback
size_t led_get_data_callback(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
    // read current led state
    bool state = digitalRead(LED_BUILTIN);
    // return pointer to data
    *data = (uint8_t*)((state) ? "1\r\n" : "0\r\n");
    // return data size
    return strlen((char*)(*data));
}

// led file set data callback
void led_set_data_callback(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
    // data size validation
    if (size < 1)
        return;

    // arguments validation
    if (data[0] == '1') {
        // turn led on
        digitalWrite(LED_BUILTIN, HIGH);
    } else if (data[0] == '0') {
        // turn led off
        digitalWrite(LED_BUILTIN, LOW);
    }
}

// time file get data callback
size_t time_get_data_callback(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
    static char time_buf[16];
    // read current time
    long current_time = millis();
    // convert
    snprintf(time_buf, sizeof(time_buf), "%ld\r\n", current_time);
    time_buf[sizeof(time_buf) - 1] = 0;
    // return pointer to data
    *data = (uint8_t*)time_buf;
    // return data size
    return strlen((char*)(*data));
}

// dev directory files descriptor
static const struct ush_file_descriptor dev_files[] = {
    {
        .name = "led",
        .description = NULL,
        .help = NULL,
        .exec = NULL,
        .get_data = led_get_data_callback,      // optional data getter callback
        .set_data = led_set_data_callback,      // optional data setter callback
    },
    {
        .name = "time",
        .description = NULL,
        .help = NULL,
        .exec = NULL,
        .get_data = time_get_data_callback,
    },
};

// dev directory handler
static struct ush_node_object dev;

extern struct ush_object ush;

void shell_dev_mount(void)
{
    // mount dev directory
    ush_node_mount(&ush, "/dev", &dev, dev_files, sizeof(dev_files) / sizeof(dev_files[0]));
}