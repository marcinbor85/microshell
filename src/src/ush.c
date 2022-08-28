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

#include "inc/ush.h"
#include "inc/ush_node.h"
#include "inc/ush_internal.h"
#include "inc/ush_shell.h"

#include <stdarg.h>
#include <string.h>

void ush_init(struct ush_object *self, const struct ush_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(desc != NULL);

        USH_ASSERT(desc->input_buffer != NULL);
        USH_ASSERT(desc->input_buffer_size > 0);
        USH_ASSERT(desc->output_buffer != NULL);
        USH_ASSERT(desc->output_buffer_size >= 4); /* internal echo buffer */

        USH_ASSERT(desc->io != NULL);
        USH_ASSERT(desc->io->read != NULL);
        USH_ASSERT(desc->io->write != NULL);

        USH_ASSERT(desc->hostname != NULL);

        self->desc = desc;
        self->root = NULL;

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1
        ush_status_t stat = ush_commands_add(self, &self->buildin_commands, g_ush_buildin_commands, g_ush_buildin_commands_num);
        if (stat != USH_STATUS_OK) {
                USH_ASSERT(false);
        }
#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */

        ush_reset(self);
}

void ush_deinit(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        ush_node_deinit_recursive(self, self->root);
        memset(self, 0, sizeof(struct ush_object));
}

bool ush_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(self->current_node != NULL);

        bool busy = false;

        if (ush_reset_service(self) != false)
                return true;        
        if (ush_prompt_service(self) != false)
                return true;        
        if (ush_read_service(self, &busy) != false)
                return busy;
#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1
        if (ush_autocomp_service(self) != false)
                return true;
#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
        if (ush_parse_service(self) != false)
                return true;        
        if (ush_write_service(self) != false)
                return true;
        if (ush_process_service(self) != false)
                return true;

        return false;
}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(status < USH_STATUS__TOTAL_NUM);

        char *ret = (char*)ush_utils_get_status_string(status);
        ush_write_pointer(self, ret, USH_STATE_RESET);
}

void ush_print(struct ush_object *self, char *buf)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(buf != NULL);

        ush_write_pointer(self, buf, USH_STATE_RESET);
}

void ush_printf(struct ush_object *self, const char *format, ...)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(format != NULL);

        /* Make sure that the default output_buffer always is used */
        if (self->write_buf != self->desc->output_buffer) {
                /* Reset output_buffer to start position */
                self->desc->output_buffer[0] = '\0';
                ush_write_pointer(self, self->desc->output_buffer, USH_STATE_RESET_PROMPT);
        } else {
                /* Allow concatenation to output_buffer */
                self->state = USH_STATE_WRITE_CHAR;
                self->write_next_state = USH_STATE_RESET_PROMPT;
        }

        /* Concatenate string to the output_buffer pointed to by self->write_buf */
        va_list arg_list;
        va_start(arg_list, format);
        size_t insert_idx = strlen(self->write_buf);
        size_t write_size_max = self->desc->output_buffer_size - insert_idx;
        int written_size_ideal = vsnprintf(&self->write_buf[insert_idx], write_size_max, format, arg_list);
        int written_size_real = strlen(&self->write_buf[insert_idx]);
        self->write_size += written_size_real;

        /* Handle possible vsnprintf() problems */
        if (written_size_ideal < 0 || /* Format error */
            written_size_ideal > written_size_real) /* Available output_buffer to small */
        {
                int error_msg_size;
                const char *error_msg;
                const char error_format[] = "...format error\r\n";
                const char error_overflow[] = "...overflow error\r\n";

                if (written_size_ideal < 0) {
                        error_msg = error_format;
                        error_msg_size = sizeof(error_format);
                } else {
                        error_msg = error_overflow;
                        error_msg_size = sizeof(error_overflow);
                }

                /* Do the error message fit into output_buffer pointed to by self->write_buf? */
                if (self->desc->output_buffer_size - strlen(self->write_buf) >= strlen(error_msg)) {
                        /* Error message fits */
                        size_t end_idx = strlen(self->write_buf);
                        (void)snprintf(&self->write_buf[end_idx], error_msg_size, "%s", error_msg);
                        self->write_size += strlen(&self->write_buf[end_idx]);
                } else {
                        /* Error message does NOT fit */
                        size_t end_idx = self->desc->output_buffer_size - error_msg_size;
                        (void)snprintf(&self->write_buf[end_idx], error_msg_size, "%s", error_msg);
                        self->write_size = self->desc->output_buffer_size;
                }
        }

        va_end(arg_list);
}

void ush_print_no_newline(struct ush_object *self, char *buf)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(buf != NULL);

        ush_write_pointer(self, buf, USH_STATE_RESET_PROMPT);
}

void ush_flush(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        ush_write_pointer(self, self->desc->output_buffer, USH_STATE_RESET_PROMPT);
}
