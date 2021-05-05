#include <microshell.h>

#include "test_func.h"

char g_read_buf[TEST_FUNC_IO_BUFFER_SIZE];
char g_write_buf[TEST_FUNC_IO_BUFFER_SIZE];

int g_write_buf_index;
int g_read_buf_index;
int g_read_buf_size;

uint8_t g_file_buffer_buf[128];
size_t g_file_buffer_buf_size;

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
static char g_input_buffer[TEST_FUNC_WORK_BUFFER_SIZE];
static char g_output_buffer[TEST_FUNC_WORK_BUFFER_SIZE];

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

void file_root_test_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)file;

        ush_print(self, "test_exec_callback");
}

struct ush_node_object g_path_root;
const struct ush_file_descriptor g_path_root_desc[] = {
        {
                .name = "test",
                .description = "test file",
                .exec = file_root_test_callback,
                .help = "nothing special\r\n",
        },
};

size_t file_null_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;
        (void)data;
        
        return 0;
}

void file_null_data_setter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        (void)self;
        (void)file;
        (void)data;
        (void)size;

        return;
}

size_t file_buffer_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        *data = g_file_buffer_buf;        
        return g_file_buffer_buf_size;
}

void file_buffer_data_setter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        (void)self;
        (void)file;

        if (size > sizeof(g_file_buffer_buf))
                size = sizeof(g_file_buffer_buf);
        memcpy(g_file_buffer_buf, data, size);

        return;
}

size_t file_binary_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        static uint8_t buf[256];

        for (size_t i = 0; i < sizeof(buf); i++)
                buf[i] = i;

        *data = buf;   
        return sizeof(buf);
}

const struct ush_file_descriptor g_path_dir111_desc[] = {
        {
                .name = "joke",
        },
};

struct ush_node_object g_path_data;
const struct ush_file_descriptor g_path_data_desc[] = {
        {
                .name = "text",
                .exec = NULL,
        },
        {
                .name = "text_file1",
                .exec = NULL,
        },
        {
                .name = "text_file2",
                .exec = NULL,
        },
        {
                .name = "null",
                .exec = NULL,
                .get_data = file_null_data_getter,
                .set_data = file_null_data_setter,
        },
        {
                .name = "buffer",
                .exec = NULL,
                .get_data = file_buffer_data_getter,
                .set_data = file_buffer_data_setter,
        },
        {
                .name = "binary",
                .exec = NULL,
                .get_data = file_binary_data_getter,
        },
};

struct ush_node_object g_path_dir;
static struct ush_node_object g_path_dir1;
static struct ush_node_object g_path_dir2;
struct ush_node_object g_path_dir11;
struct ush_node_object g_path_dir111;
static struct ush_node_object g_path_dir12;
static struct ush_node_object g_path_dir13;
static struct ush_node_object g_path_dir21;
static struct ush_node_object g_path_dir211;
static struct ush_node_object g_path_dir212;

void test_func_init(void)
{
        memset(g_file_buffer_buf, 0, sizeof(g_file_buffer_buf));
        g_file_buffer_buf_size = 0;
        memset(g_input_buffer, 0, sizeof(g_input_buffer));
        memset(g_output_buffer, 0, sizeof(g_output_buffer));

        g_read_buf_size = 0;
        g_read_buf_index = 0;
        g_write_buf_index = 0;
        ush_init(&g_ush, &g_ush_desc);

        ush_node_mount(&g_ush, "/", &g_path_root, g_path_root_desc, sizeof(g_path_root_desc) / sizeof(g_path_root_desc[0]));
        ush_node_mount(&g_ush, "/data", &g_path_data, g_path_data_desc, sizeof(g_path_data_desc) / sizeof(g_path_data_desc[0]));
        ush_node_mount(&g_ush, "/dir", &g_path_dir, NULL, 0);
        ush_node_mount(&g_ush, "/dir/1", &g_path_dir1, NULL, 0);
        ush_node_mount(&g_ush, "/dir/2", &g_path_dir2, NULL, 0);
        ush_node_mount(&g_ush, "/dir/1/11", &g_path_dir11, NULL, 0);
        ush_node_mount(&g_ush, "/dir/1/11/111", &g_path_dir111, g_path_dir111_desc, sizeof(g_path_dir111_desc) / sizeof(g_path_dir111_desc[0]));
        ush_node_mount(&g_ush, "/dir/1/12", &g_path_dir12, NULL, 0);
        ush_node_mount(&g_ush, "/dir/1/13", &g_path_dir13, NULL, 0);
        ush_node_mount(&g_ush, "/dir/2/21", &g_path_dir21, NULL, 0);
        ush_node_mount(&g_ush, "/dir/2/21/211", &g_path_dir211, NULL, 0);
        ush_node_mount(&g_ush, "/dir/2/21/212", &g_path_dir212, NULL, 0);

        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                USH_NAME " " USH_VERSION "\r\n"
                "[test /]$ ",
                g_write_buf
        );
}

void test_func_deinit(void)
{
        ush_deinit(&g_ush);
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
