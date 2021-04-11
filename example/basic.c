#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "ush.h"

static FILE *g_io_read;
static FILE *g_io_write;

struct termios g_saved_attributes;

void reset_input_mode(void)
{
        tcsetattr(STDIN_FILENO, TCSANOW, &g_saved_attributes);
}

void set_input_mode(void)
{
        struct termios tattr;

        if (!isatty (STDIN_FILENO)) {
                fprintf(stderr, "not a terminal\n");
                exit(EXIT_FAILURE);
        }

        tcgetattr(STDIN_FILENO, &g_saved_attributes);
        atexit(reset_input_mode);

        tcgetattr (STDIN_FILENO, &tattr);
        tattr.c_lflag &= ~(ICANON|ECHO);
        tattr.c_cc[VMIN] = 1;
        tattr.c_cc[VTIME] = 0;
        tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

static int write_char(struct ush_object *self, char ch)
{
        (void)self;

        char c = fputc(ch, g_io_write);
        if (c != ch)
                return 0;
        return 1;
}

static int read_char(struct ush_object *self, char *ch)
{
        (void)self;
     
        *ch = fgetc(g_io_read);
        return 1;
}

static const struct ush_io_interface g_ush_io_interface = {
        .read = read_char,
        .write = write_char,
};

// static void exec(struct ush_object *self, int argc, char *argv[])
// {
//         (void)self;
        
//         int i;
//         static char buf[128];
//         memset(buf, 0, sizeof(buf));
//         for (i = 0; i < argc; i++) {
//                 strcat(buf, argv[i]);
//                 strcat(buf, "\r\n");
//         }
//         ush_write_pointer(self, buf, USH_STATE_RESET);
// }

static char g_input_buffer[256];

static const struct ush_descriptor g_ush_desc = {
        .io = &g_ush_io_interface,
        .input_buffer = g_input_buffer,
        .input_buffer_size = sizeof(g_input_buffer),
        .hostname = "host",
        // .exec = exec,
};
static struct ush_object g_ush;

static void g_print_name_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)argc;
        (void)argv;

        ush_print(self, (char*)file->name);
}

static const struct ush_file_descriptor g_path_global_desc2[] = {
        {
                .name = "test",
                .description = "test command",
                .exec = g_print_name_callback,
        },
        {
                .name = "test_more1",
                .description = "test_more1 command",
                .exec = g_print_name_callback,
        },
        {
                .name = "test_more2",
                .description = "test_more2 command",
                .exec = g_print_name_callback,
        }
};

static struct ush_node_object g_path_global2;

static const struct ush_file_descriptor g_path_root_desc[] = {
        {
                .name = "start",
                .description = "start device",
                .exec = g_print_name_callback,
        },
        {
                .name = "stop",
                .description = "stop device",
                .exec = g_print_name_callback,
        }
};

static struct ush_node_object g_path_root;

static const struct ush_file_descriptor g_path_dev_desc[] = {
        {
                .name = "gpio_write",
                .description = "write to gpio",
                .exec = g_print_name_callback,
        },
        {
                .name = "gpio_read",
                .description = "read from gpio",
                .exec = g_print_name_callback,
        }
};

static struct ush_node_object g_path_dev;

static const struct ush_file_descriptor g_path_etc_desc[] = {
        {
                .name = "config",
                .description = "configuration",
        },
};

static struct ush_node_object g_path_etc;

static const struct ush_file_descriptor g_path_dev_bus_desc[] = {
        {
                .name = "spi",
                .description = "show spi",
                .exec = g_print_name_callback,
        },
        {
                .name = "i2c",
                .description = "show i2c",
                .exec = g_print_name_callback,
        }
};

static struct ush_node_object g_path_dev_bus;

static const struct ush_file_descriptor g_path_dev_mem_desc[] = {
        {
                .name = "ram",
                .description = "show ram memory",
                .exec = g_print_name_callback,
        }
};

static struct ush_node_object g_path_dev_mem;

static const struct ush_file_descriptor g_path_dev_mem_ext_desc[] = {
        {
                .name = "flash",
                .description = "show flash memory",
                .exec = g_print_name_callback,
        },
        {
                .name = "disk",
                .exec = g_print_name_callback,
        }
};

static struct ush_node_object g_path_dev_mem_ext;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        if (argc == 1) {
                g_io_read = stdin;
                g_io_write = stdout;
                set_input_mode();
        } else if (argc == 2) {
                g_io_read = fopen(argv[1], "a+");
                if (g_io_read == NULL) {
                        fprintf(stderr, "cannot open device\n");
                        exit(EXIT_FAILURE);
                }
                g_io_write = g_io_read;
        } else {
                fprintf(stderr, "wrong arguments\n");
                exit(EXIT_FAILURE);
        }
        
        ush_init(&g_ush, &g_ush_desc);

        ush_commands_add(&g_ush, &g_path_global2, g_path_global_desc2, sizeof(g_path_global_desc2) / sizeof(g_path_global_desc2[0]));

        ush_node_mount(&g_ush, "/", &g_path_root, g_path_root_desc, sizeof(g_path_root_desc) / sizeof(g_path_root_desc[0]));
        ush_node_mount(&g_ush, "/dev", &g_path_dev, g_path_dev_desc, sizeof(g_path_dev_desc) / sizeof(g_path_dev_desc[0]));
        ush_node_mount(&g_ush, "/etc", &g_path_etc, g_path_etc_desc, sizeof(g_path_etc_desc) / sizeof(g_path_etc_desc[0]));
        ush_node_mount(&g_ush, "/dev/bus", &g_path_dev_bus, g_path_dev_bus_desc, sizeof(g_path_dev_bus_desc) / sizeof(g_path_dev_bus_desc[0]));
        ush_node_mount(&g_ush, "/dev/mem", &g_path_dev_mem, g_path_dev_mem_desc, sizeof(g_path_dev_mem_desc) / sizeof(g_path_dev_mem_desc[0]));
        ush_node_mount(&g_ush, "/dev/mem/external", &g_path_dev_mem_ext, g_path_dev_mem_ext_desc, sizeof(g_path_dev_mem_ext_desc) / sizeof(g_path_dev_mem_ext_desc[0]));
        
        // ush_node_unmount(&g_ush, "/dev/mem/external");

        ush_node_set_current_dir(&g_ush, "/");

        while (1) {
                ush_service(&g_ush);
        }
        reset_input_mode();
        return 0;
}
