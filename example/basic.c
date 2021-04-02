#include <stdio.h>

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

static const struct ush_io_interface g_ush_iface = {
        .read = read_char,
        .write = write_char,
};

static char g_cmd_buffer[256];

static const struct ush_descriptor g_ush_desc = {
        .iface = &g_ush_iface,
        .cmd_buffer = g_cmd_buffer,
        .cmd_buffer_size = 0,//sizeof(g_cmd_buffer),
};

static struct ush_object g_ush;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        ush_init(&g_ush, &g_ush_desc);
        return 0;
}
