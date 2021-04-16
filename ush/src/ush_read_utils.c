#include "ush_internal.h"

void ush_read_echo_service(struct ush_object *self, char ch)
{
        switch (ch) {
        case '\r':
                self->desc->output_buffer[0] = ch;
                self->desc->output_buffer[1] = '\n';
                self->desc->output_buffer[2] = '\0';
                break;
        case '\x08':
        case '\x7F':
                self->desc->output_buffer[0] = '\x08';
                self->desc->output_buffer[1] = ' ';
                self->desc->output_buffer[2] = '\x08';
                self->desc->output_buffer[3] = '\0';
                break;
        default:
                self->desc->output_buffer[0] = ch;
                self->desc->output_buffer[1] = '\0';
                break;
        }

        ush_state_t next = (ch == '\r' || ch == '\n') ? USH_STATE_PARSE_PREPARE : USH_STATE_READ_CHAR;
        ush_write_pointer(self, self->desc->output_buffer, next);
}

bool ush_read_char_by_escape_state(struct ush_object *self, char ch)
{
        bool echo = true;

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

        return echo;
}

void ush_read_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->in_pos = 0;
        self->desc->input_buffer[self->in_pos] = '\0';
        self->state = USH_STATE_READ_CHAR;
        self->ansi_escape_state = 0;
}
