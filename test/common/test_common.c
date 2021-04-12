#include "test_common.h"

char g_write_buffer[TEST_COMMON_EMULATE_IO_BUFFER_SIZE];
size_t g_write_buffer_index;
char g_read_buffer[TEST_COMMON_EMULATE_IO_BUFFER_SIZE];
size_t g_read_buffer_index;
size_t g_read_buffer_pos;

static int write_char(struct ush_object *self, char ch)
{
        (void)self;

        if (g_write_buffer_index >= sizeof(g_write_buffer))
                return 0;
        g_write_buffer[g_write_buffer_index++] = ch;
        return 1;
}

static int read_char(struct ush_object *self, char *ch)
{
        (void)self;

        if (g_read_buffer_index >= g_read_buffer_pos)
                return 0;
        *ch = g_read_buffer[g_read_buffer_index++];
        return 1;
}

static const struct ush_io_interface g_ush_iface = {
        .read = read_char,
        .write = write_char,
};

static char g_input_buffer[TEST_COMMON_INPUT_BUFFER_SIZE];
static char g_output_buffer[TEST_COMMON_OUTPUT_BUFFER_SIZE];

static const struct ush_descriptor g_ush_desc = {
        .io = &g_ush_iface,
        .input_buffer = g_input_buffer,
        .input_buffer_size = sizeof(g_input_buffer),
        .output_buffer = g_output_buffer,
        .output_buffer_size = sizeof(g_output_buffer),
};

struct ush_object g_ush;

void test_common_init(void)
{
        ush_init(&g_ush, &g_ush_desc);
}

void test_common_reset(void)
{
        g_write_buffer_index = 0;
        memset(g_write_buffer, 0, sizeof(g_write_buffer));

        g_read_buffer_index = 0;
        g_read_buffer_pos = 0;
        memset(g_read_buffer, 0, sizeof(g_read_buffer));
}
