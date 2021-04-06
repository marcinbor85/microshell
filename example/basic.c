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

static char* g_cmd_help_callback(struct ush_object *self, struct ush_cmd_object *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        struct ush_cmd_object *curr = self->cmd_first;

        while (curr != NULL) {
                strcat(buf, curr->desc->name);
                strcat(buf, "\t");
                strcat(buf, curr->desc->description);
                strcat(buf, "\r\n");
                curr = curr->next;
        }
        
        return buf;
}


static char* g_cmd_list_callback(struct ush_object *self, struct ush_cmd_object *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        struct ush_cmd_object *curr = self->cmd_first;

        while (curr != NULL) {
                if (strcmp(curr->desc->path, self->current_dir) == 0) {
                        strcat(buf, curr->desc->name);
                        strcat(buf, "\t");
                        strcat(buf, curr->desc->description);
                        strcat(buf, "\r\n");
                }
                
                curr = curr->next;
        }
        
        return buf;
}

static char* g_cmd_cd_callback(struct ush_object *self, struct ush_cmd_object *cmd, int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        (void)cmd;

        if (argc != 2) {
                return "error\r\n";
        }

        strcpy(self->current_dir, argv[1]);
        return NULL;
}

static char* g_print_name_callback(struct ush_object *self, struct ush_cmd_object *cmd, int argc, char *argv[])
{
        (void)self;
        (void)argc;
        (void)argv;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        strcat(buf, cmd->desc->name);
        strcat(buf, "\r\n");

        return buf;
}

static const struct ush_cmd_descriptor g_cmd_help_desc = {
        .path = "",
        .name = "help",
        .description = "print available commands",
        .cmd_callback = g_cmd_help_callback,
};

static struct ush_cmd_object g_cmd_help;

static const struct ush_cmd_descriptor g_cmd_list_desc = {
        .path = "",
        .name = "list",
        .description = "print commands in current directory",
        .cmd_callback = g_cmd_list_callback,
};

static struct ush_cmd_object g_cmd_list;

static const struct ush_cmd_descriptor g_cmd_cd_desc = {
        .path = "",
        .name = "cd",
        .description = "change current directory",
        .cmd_callback = g_cmd_cd_callback,
};

static struct ush_cmd_object g_cmd_cd;

static const struct ush_cmd_descriptor g_cmd_root_start_desc = {
        .path = "/",
        .name = "start",
        .description = "start device",
        .cmd_callback = g_print_name_callback,
};

static struct ush_cmd_object g_cmd_root_start;

static const struct ush_cmd_descriptor g_cmd_root_stop_desc = {
        .path = "/",
        .name = "stop",
        .description = "stop device",
        .cmd_callback = g_print_name_callback,
};

static struct ush_cmd_object g_cmd_root_stop;

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

        ush_cmd_register(&g_ush, &g_cmd_help, &g_cmd_help_desc);
        ush_cmd_register(&g_ush, &g_cmd_list, &g_cmd_list_desc);
        ush_cmd_register(&g_ush, &g_cmd_cd, &g_cmd_cd_desc);
        ush_cmd_register(&g_ush, &g_cmd_root_start, &g_cmd_root_start_desc);
        ush_cmd_register(&g_ush, &g_cmd_root_stop, &g_cmd_root_stop_desc);

        while (1) {
                ush_service(&g_ush);
        }
        return 0;
}
