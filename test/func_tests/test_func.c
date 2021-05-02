#include "ush.h"
#include "test_func.h"

char g_read_buf[1024];
char g_write_buf[1024];

int g_write_buf_index;
int g_read_buf_index;
int g_read_buf_size;

static int write_char(struct ush_object *self, char ch)
{
        (void)self;

        g_write_buf[g_write_buf_index++] = ch;
        return 1;
}

static int read_char(struct ush_object *self, char *ch)
{
        (void)self;
     
        if (g_read_buf_index < g_read_buf_size) {
                *ch = g_read_buf[g_read_buf_index++];
                return 1;
        }

        return 0;
}

static const struct ush_io_interface g_ush_io_interface = {
        .read = read_char,
        .write = write_char,
};

struct ush_object g_ush;
static char g_input_buffer[256];
static char g_output_buffer[256];

static char g_hostname_data[16] = {
        "test"
};

static const struct ush_descriptor g_ush_desc = {
        .io = &g_ush_io_interface,
        .input_buffer = g_input_buffer,
        .input_buffer_size = sizeof(g_input_buffer),
        .output_buffer = g_output_buffer,
        .output_buffer_size = sizeof(g_output_buffer),
        .hostname = g_hostname_data,
};

static struct ush_node_object g_path_root;
static const struct ush_file_descriptor g_path_root_desc[] = {
        {
                .name = "test",
                .description = "test file",
                .exec = NULL,
        },
};

void test_func_init(void)
{
        g_read_buf_size = 0;
        g_read_buf_index = 0;
        g_write_buf_index = 0;
        ush_init(&g_ush, &g_ush_desc);

        ush_node_mount(&g_ush, "/", &g_path_root, g_path_root_desc, sizeof(g_path_root_desc) / sizeof(g_path_root_desc[0]));

        ush_node_set_current_dir(&g_ush, "/");

        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "uShell 0.1.0\r\n"
                "[test /]$ ",
                g_write_buf
        );
}

void test_func_write(const char *text)
{
        g_read_buf_size = strlen(text);
        g_read_buf_index = 0;
        strcpy(g_read_buf, text);
}

void test_func_read_all(void)
{
        g_write_buf_index = 0;
        while (ush_service(&g_ush) != false) {};
        g_write_buf[g_write_buf_index] = '\0';
}

void test_func_ask(const char *request, const char *response)
{
        char buf[1024];
        char buf_resp[1024];

        sprintf(buf, "%s\n", request);
        sprintf(buf_resp, "%s\n%s", request, response);
        test_func_write(buf);
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                buf_resp,
                g_write_buf
        );
}
