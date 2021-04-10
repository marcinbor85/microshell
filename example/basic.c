#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "ush.h"
#include "ush_shell.h"

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
                fprintf(stderr, "Not a terminal.\n");
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

// static char* exec(struct ush_object *self, int argc, char *argv[])
// {
//         (void)self;
        
//         int i;
//         static char buf[128];
//         memset(buf, 0, sizeof(buf));
//         for (i = 0; i < argc; i++) {
//                 strcat(buf, argv[i]);
//                 strcat(buf, "\r\n");
//         }
//         return buf;
// }

static char g_input_buffer[256];
static char g_output_buffer[256];

static const struct ush_descriptor g_ush_desc = {
        .io = &g_ush_io_interface,
        .input_buffer = g_input_buffer,
        .input_buffer_size = sizeof(g_input_buffer),
        .output_buffer = g_output_buffer,
        .output_buffer_size = sizeof(g_output_buffer),
        .hostname = "host",
        // .exec = exec,
};
static struct ush_object g_ush;

static char* g_file_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)file;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        if (argc == 1) {
                struct ush_node_object *curr = self->root;

                while (curr != NULL) {
                        if (curr->path != NULL) {
                                curr = curr->next;
                                continue;
                        }

                        for (size_t i = 0; i < curr->file_list_size; i++) {
                                struct ush_file_descriptor const *file = &curr->file_list[i];
                                strcat(buf, file->name);
                                if (file->description != NULL) {
                                        strcat(buf, "\t- ");
                                        strcat(buf, file->description);
                                }
                                strcat(buf, "\r\n");
                        }

                        curr = curr->next;
                }
        } else if (argc == 2) {
                struct ush_file_descriptor const *help_cmd = ush_file_find_by_name(self, argv[1]);
                if (help_cmd == NULL) 
                        return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_COMMAND_NOT_FOUND);

                if (help_cmd->help == NULL)
                        return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_NO_HELP_AVAILABLE);

                strcat(buf, help_cmd->help);
        } else {
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);
        }

        return buf;
}


static char* g_file_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)file;

        if (argc > 2)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);
        
        static char buf[512];
        memset(buf, 0, sizeof(buf));

        // char path_dir[512];
        // struct ush_node_object *path_obj;

        // if (argc == 1) {
        //         ush_path_get_current_dir(self, path_dir, sizeof(path_dir));
        //         path_obj = self->current_node;
        // } else {
        //         ush_path_get_absolute_path(self, argv[1], path_dir, sizeof(path_dir));

        //         path_obj = ush_path_by_mount_point(self, path_dir);
        //         if (path_obj == NULL)
        //                 return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_DIRECTORY_NOT_FOUND);

        //         ush_path_get_full_path(self, path_obj, path_dir, sizeof(path_dir));
        // }

        // struct ush_node_object *curr = self->root;

        // strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        // strcat(buf, ".");
        // strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

        // if (path_obj->name[0] != '\0') {
        //         strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        //         strcat(buf, "..");
        //         strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");
        // }

        // /* first - directories only in current path */
        // while (curr != NULL) {
        //         if ((curr == path_obj) || (curr->path == NULL) || (strcmp(curr->path, path_dir) != 0)) {
        //                 curr = curr->next;
        //                 continue;
        //         }

        //         strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        //         strcat(buf, curr->name);
        //         strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

        //         curr = curr->next;
        // }

        // /* next - files in current path */
        // for (size_t i = 0; i < path_obj->file_list_size; i++) {
        //         struct ush_file_descriptor const *file = &path_obj->file_list[i];
        //         strcat(buf, file->name);
        //         if (file->description != NULL) {
        //                 strcat(buf, "\t- ");
        //                 strcat(buf, file->description);
        //         }
        //         strcat(buf, "\r\n");
        // }
        
        return buf;
}

static char* g_file_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)file;

        if (argc != 2)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);

        bool success = ush_node_set_current_dir(self, argv[1]);
        if (success == false)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_DIRECTORY_NOT_FOUND);

        return NULL;
}

static char* g_file_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)file;

        if (argc != 1)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);

        static char buf[512];
        buf[0] = '\0';
        strcat(buf, self->current_node->path);
        strcat(buf, "\r\n");

        return buf;
}

static char* g_print_name_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)argc;
        (void)argv;

        static char buf[512];
        snprintf(buf, sizeof(buf), "%s\r\n", file->name);
        return buf;
}

static const struct ush_file_descriptor g_path_global_desc[] = {
        {
                .name = "help",
                .description = "list available commands",
                .help = "help: help [file]\r\n\tShow help information for file or command.\r\n"
                        "      help\r\n\tShow available commands.\r\n",
                .exec = g_file_help_callback,
        },
        {
                .name = "ls",
                .description = "list directory content",
                .help = "ls: ls [path]\r\n\tList directory content.\r\n"
                        "    ls\r\n\tList current directory content.\r\n",
                .exec = g_file_ls_callback,
        },
        {
                .name = "cd",
                .description = "change current directory",
                .help = "cd: cd [path]\r\n\tChange current working directory.\r\n",
                .exec = g_file_cd_callback,
        },
        {
                .name = "pwd",
                .description = "print current directory",
                .help = "pwd: pwd\r\n\tPrint current working directory path.\r\n",
                .exec = g_file_pwd_callback,
        }
};

static struct ush_node_object g_path_global;

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
                .description = "show disk memory",
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
                        return -1;
                }
                g_io_write = g_io_read;
        } else {
                fprintf(stderr, "wrong arguments\n");
                return -1;
        }
        
        ush_init(&g_ush, &g_ush_desc);

        ush_node_mount(&g_ush, NULL, &g_path_global, g_path_global_desc, sizeof(g_path_global_desc) / sizeof(g_path_global_desc[0]));
        ush_node_mount(&g_ush, "/", &g_path_root, g_path_root_desc, sizeof(g_path_root_desc) / sizeof(g_path_root_desc[0]));
        ush_node_mount(&g_ush, "/dev", &g_path_dev, g_path_dev_desc, sizeof(g_path_dev_desc) / sizeof(g_path_dev_desc[0]));
        ush_node_mount(&g_ush, "/etc", &g_path_etc, g_path_etc_desc, sizeof(g_path_etc_desc) / sizeof(g_path_etc_desc[0]));
        ush_node_mount(&g_ush, "/dev/bus", &g_path_dev_bus, g_path_dev_bus_desc, sizeof(g_path_dev_bus_desc) / sizeof(g_path_dev_bus_desc[0]));
        ush_node_mount(&g_ush, "/dev/mem", &g_path_dev_mem, g_path_dev_mem_desc, sizeof(g_path_dev_mem_desc) / sizeof(g_path_dev_mem_desc[0]));
        ush_node_mount(&g_ush, "/dev/mem/external", &g_path_dev_mem_ext, g_path_dev_mem_ext_desc, sizeof(g_path_dev_mem_ext_desc) / sizeof(g_path_dev_mem_ext_desc[0]));
        
        ush_node_set_current_dir(&g_ush, "/");

        while (1) {
                ush_service(&g_ush);
        }
        reset_input_mode();
        return 0;
}
