#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "ush.h"

static int write_char(struct ush_object *self, char ch)
{
        (void)self;

        putc(ch, stdout);
        return 1;
}

static int read_char(struct ush_object *self, char *ch)
{
        (void)self;
     
        *ch = getc(stdin);
        return 1;
}

static const struct ush_iface g_ush_iface = {
        .read = read_char,
        .write = write_char,
};

static char g_input_buffer[256];
static char g_output_buffer[256];

static const struct ush_descriptor g_ush_desc = {
        .iface = &g_ush_iface,
        .input_buffer = g_input_buffer,
        .input_buffer_size = sizeof(g_input_buffer),
        .output_buffer = g_output_buffer,
        .output_buffer_size = sizeof(g_output_buffer),
        .hostname = "host",
};
static struct ush_object g_ush;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        ush_init(&g_ush, &g_ush_desc);

        while (1) {
                ush_service(&g_ush);
        }
        return 0;
}
