#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "ush.h"
#include "ush_shell.h"

static FILE *g_io;

static int write_char(struct ush_object *self, char ch)
{
        (void)self;
        char c;

        c = fputc(ch, g_io);
        if (c != ch)
                return 0;
        return 1;
}

static int read_char(struct ush_object *self, char *ch)
{
        (void)self;
     
        *ch = fgetc(g_io);
        return 1;
}

static const struct ush_io_interface g_ush_io_interface = {
        .read = read_char,
        .write = write_char,
};

// static char* cmd_callback(struct ush_object *self, int argc, char *argv[])
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
        // .cmd_callback = cmd_callback,
};
static struct ush_object g_ush;

static char* g_cmd_help_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        if (argc == 1) {
                struct ush_path_object *curr = self->path_first;

                while (curr != NULL) {
                        if (curr->mount_point != NULL) {
                                curr = curr->next;
                                continue;
                        }

                        for (size_t i = 0; i < curr->cmd_list_size; i++) {
                                struct ush_cmd_descriptor const *cmd = &curr->cmd_list[i];
                                strcat(buf, cmd->name);
                                strcat(buf, "\t");
                                if (cmd->description != NULL)
                                        strcat(buf, cmd->description);
                                strcat(buf, "\r\n");
                        }

                        curr = curr->next;
                }
        } else if (argc == 2) {
                struct ush_cmd_descriptor const *help_cmd = ush_cmd_find_by_name(self, argv[1]);
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


static char* g_cmd_ls_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        if (argc != 1)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        char current_dir[512];
        ush_path_get_current_dir(self, current_dir, sizeof(current_dir));

        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                if ((curr == self->current_path) || (curr->mount_point == NULL) || (strcmp(curr->mount_point, current_dir) != 0)) {
                        curr = curr->next;
                        continue;
                }

                strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
                strcat(buf, curr->name);
                strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

                curr = curr->next;
        }

        for (size_t i = 0; i < self->current_path->cmd_list_size; i++) {
                struct ush_cmd_descriptor const *cmd = &self->current_path->cmd_list[i];
                strcat(buf, cmd->name);
                strcat(buf, "\t");
                if (cmd->description != NULL)
                        strcat(buf, cmd->description);
                strcat(buf, "\r\n");
        }
        
        return buf;
}

static char* g_cmd_cd_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        if (argc != 2)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);

        bool success = ush_path_set_current_dir(self, argv[1]);
        if (success == false)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_DIRECTORY_NOT_FOUND);

        return NULL;
}

static char* g_cmd_pwd_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        if (argc != 1)
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);

        static char buf[512];
        ush_path_get_current_dir(self, buf, sizeof(buf));
        strcat(buf, "\r\n");
        return buf;
}

static char* g_print_name_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)self;
        (void)argc;
        (void)argv;

        static char buf[512];
        snprintf(buf, sizeof(buf), "%s\r\n", cmd->name);
        return buf;
}

static const struct ush_cmd_descriptor g_path_global_desc[] = {
        {
                .name = "help",
                .description = "print available commands",
                .help = "help: help [cmd]\r\n\tShow help information for file or command.\r\n",
                .cmd_callback = g_cmd_help_callback,
        },
        {
                .name = "ls",
                .description = "print current directory content",
                .help = "ls: ls\r\n\tPrint current directory content.\r\n",
                .cmd_callback = g_cmd_ls_callback,
        },
        {
                .name = "cd",
                .description = "change current directory",
                .help = "cd: cd [path]\r\n\tChange current working directory.\r\n",
                .cmd_callback = g_cmd_cd_callback,
        },
        {
                .name = "pwd",
                .description = "print current directory",
                .help = "pwd: pwd\r\n\tPrint current working directory path.\r\n",
                .cmd_callback = g_cmd_pwd_callback,
        }
};

static struct ush_path_object g_path_global;

static const struct ush_cmd_descriptor g_path_root_desc[] = {
        {
                .name = "start",
                .description = "start device",
                .cmd_callback = g_print_name_callback,
        },
        {
                .name = "stop",
                .description = "stop device",
                .cmd_callback = g_print_name_callback,
        }
};

static struct ush_path_object g_path_root;

static const struct ush_cmd_descriptor g_path_dev_desc[] = {
        {
                .name = "gpio_write",
                .description = "write to gpio",
                .cmd_callback = g_print_name_callback,
        },
        {
                .name = "gpio_read",
                .description = "read from gpio",
                .cmd_callback = g_print_name_callback,
        }
};

static struct ush_path_object g_path_dev;

static const struct ush_cmd_descriptor g_path_dev_bus_desc[] = {
        {
                .name = "spi",
                .description = "show spi",
                .cmd_callback = g_print_name_callback,
        },
        {
                .name = "i2c",
                .description = "show i2c",
                .cmd_callback = g_print_name_callback,
        }
};

static struct ush_path_object g_path_dev_bus;

static const struct ush_cmd_descriptor g_path_dev_mem_desc[] = {
        {
                .name = "ram",
                .description = "show ram memory",
                .cmd_callback = g_print_name_callback,
        }
};

static struct ush_path_object g_path_dev_mem;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        g_io = fopen(argv[1], "a+");
        if (g_io == NULL) {
                fprintf(stderr, "cannot open device\n");
                return -1;
        }
        
        ush_init(&g_ush, &g_ush_desc);

        ush_path_mount(&g_ush, NULL, NULL, &g_path_global, g_path_global_desc, sizeof(g_path_global_desc) / sizeof(g_path_global_desc[0]));
        ush_path_mount(&g_ush, "/", "", &g_path_root, g_path_root_desc, sizeof(g_path_root_desc) / sizeof(g_path_root_desc[0]));
        ush_path_mount(&g_ush, "/", "dev", &g_path_dev, g_path_dev_desc, sizeof(g_path_dev_desc) / sizeof(g_path_dev_desc[0]));
        ush_path_mount(&g_ush, "/dev", "bus", &g_path_dev_bus, g_path_dev_bus_desc, sizeof(g_path_dev_bus_desc) / sizeof(g_path_dev_bus_desc[0]));
        ush_path_mount(&g_ush, "/dev", "mem", &g_path_dev_mem, g_path_dev_mem_desc, sizeof(g_path_dev_mem_desc) / sizeof(g_path_dev_mem_desc[0]));

        ush_path_set_current_dir(&g_ush, "/");

        while (1) {
                ush_service(&g_ush);
        }
        return 0;
}
