#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "ush.h"

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

        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                if (curr->path != NULL) {
                        curr = curr->next;
                        continue;
                }

                for (size_t i = 0; i < curr->cmd_list_size; i++) {
                        struct ush_cmd_descriptor const *cmd = &curr->cmd_list[i];
                        strcat(buf, cmd->name);
                        strcat(buf, "\t");
                        strcat(buf, cmd->description);
                        strcat(buf, "\r\n");
                }
                
                break;
        }
        
        return buf;
}


static char* g_cmd_ls_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                if ((curr->path == NULL) || (strcmp(curr->path, self->current_dir) != 0)) {
                        curr = curr->next;
                        continue;
                }

                for (size_t i = 0; i < curr->cmd_list_size; i++) {
                        struct ush_cmd_descriptor const *cmd = &curr->cmd_list[i];
                        strcat(buf, cmd->name);
                        strcat(buf, "\t");
                        strcat(buf, cmd->description);
                        strcat(buf, "\r\n");
                }
                
                break;
        }
        
        return buf;
}

static char* g_cmd_cd_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        if (argc != 2) {
                return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_WRONG_ARGUMENTS);
        }

        strcpy(self->current_dir, argv[1]);
        return NULL;
}

static char* g_cmd_pwd_callback(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        static char buf[512];
        snprintf(buf, sizeof(buf), "%s\r\n", self->current_dir);
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
                .cmd_callback = g_cmd_help_callback,
        },
        {
                .name = "ls",
                .description = "print current directory content",
                .cmd_callback = g_cmd_ls_callback,
        },
        {
                .name = "cd",
                .description = "change current directory",
                .cmd_callback = g_cmd_cd_callback,
        },
        {
                .name = "pwd",
                .description = "print current directory",
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

        ush_path_register(&g_ush, NULL, &g_path_global, g_path_global_desc, sizeof(g_path_global_desc) / sizeof(g_path_global_desc[0]));
        ush_path_register(&g_ush, "/", &g_path_root, g_path_root_desc, sizeof(g_path_root_desc) / sizeof(g_path_root_desc[0]));

        while (1) {
                ush_service(&g_ush);
        }
        return 0;
}
