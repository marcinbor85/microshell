#include "ush_internal.h"
#include "ush_config.h"

void ush_read_echo_service(struct ush_object *self, char ch)
{
        switch (ch) {
        case '\r':
                self->echo_buf[0] = ch;
                self->echo_buf[1] = '\n';
                self->echo_buf[2] = '\0';
                break;
        case '\x08':
        case '\x7F':
                self->echo_buf[0] = '\x08';
                self->echo_buf[1] = ' ';
                self->echo_buf[2] = '\x08';
                self->echo_buf[3] = '\0';
                break;
        default:
                self->echo_buf[0] = ch;
                self->echo_buf[1] = '\0';
                break;
        }

        ush_state_t next = (ch == '\r' || ch == '\n') ? USH_STATE_PARSE_PREPARE : USH_STATE_READ_CHAR;
        ush_write_pointer(self, self->echo_buf, next);
}

bool ush_read_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch;
        bool echo = true;

        if (self->desc->io->read(self, &ch) == 0)
                return false;
        
        switch (ch) {
        case '\x03':
                /* ctrl+c */
                ush_write_pointer(self, "^C", USH_STATE_RESET);
                echo = false;
                break;
        case '\x08':
        case '\x7F':
                /* backspace */
                if (self->in_pos > 0) {
                        self->in_pos--;
                        self->desc->input_buffer[self->in_pos] = '\0';
                } else {
                        echo = false;
                }
                break;
        case '\x09':
                /* tab */
                ush_autocomp_start(self);
                echo = false;
                break;
        case '\x1B':
                /* escape */
                self->ansi_escape_state = 1;
                echo = false;
                break;
        default:
                if (self->ansi_escape_state == 0) {
                        self->desc->input_buffer[self->in_pos++] = ch;
                        if (self->in_pos >= self->desc->input_buffer_size)
                                self->in_pos = 0;
                        self->desc->input_buffer[self->in_pos] = '\0';
                } else if (self->ansi_escape_state == 1) {
                        /* normal or ctrl */
                        if (ch == '\x5B' || ch == '\x4F') {
                                self->ansi_escape_state = 2;
                        } else {
                                self->ansi_escape_state = 0;
                        }
                        echo = false;
                } else if (self->ansi_escape_state == 2) {
                        if (ch == '\x41') {
                                /* up */
                        } else if (ch == '\x42') {
                                /* down */
                        } else if (ch == '\x43') {
                                /* right */
                        } else if (ch == '\x44') {
                                /* left */
                        }
                        self->ansi_escape_state = 0;
                        echo = false;
                }
        }

        if (echo != false)
                ush_read_echo_service(self, ch);
        
        return true;
}

void ush_read_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->in_pos = 0;
        self->desc->input_buffer[self->in_pos] = '\0';
        self->state = USH_STATE_READ_CHAR;
        self->ansi_escape_state = 0;
}

bool ush_read_service(struct ush_object *self, bool *read)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_READ_PREPARE:
                ush_read_start(self);
                break;
        case USH_STATE_READ_CHAR:
                *read = ush_read_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
