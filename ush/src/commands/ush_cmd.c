#include "ush_types.h"
#include "ush_config.h"

extern void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_help_service(struct ush_object *self, struct ush_file_descriptor const *file);

extern void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_ls_service(struct ush_object *self, struct ush_file_descriptor const *file);

extern void ush_buildin_cmd_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);

extern void ush_buildin_cmd_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);

extern void ush_buildin_cmd_cat_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_cat_service(struct ush_object *self, struct ush_file_descriptor const *file);

extern void ush_buildin_cmd_xxd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_xxd_service(struct ush_object *self, struct ush_file_descriptor const *file);

const struct ush_file_descriptor g_ush_buildin_commands[] = {
        {
                .name = "help",
                .description = "list available commands",
                .help = "help: help [file]\r\n\tShow help information for file or command.\r\n"
                        "      help\r\n\tShow available commands.\r\n",
                .exec = ush_buildin_cmd_help_callback,
                .process = ush_buildin_cmd_help_service,
        },
        {
                .name = "ls",
                .description = "list directory content",
                .help = "ls: ls [path]\r\n\tList directory content.\r\n"
                        "    ls\r\n\tList current directory content.\r\n",
                .exec = ush_buildin_cmd_ls_callback,
                .process = ush_buildin_cmd_ls_service,
        },
        {
                .name = "cd",
                .description = "change current directory",
                .help = "cd: cd [path]\r\n\tChange current working directory.\r\n",
                .exec = ush_buildin_cmd_cd_callback,
        },
        {
                .name = "pwd",
                .description = "print current directory",
                .help = "pwd: pwd\r\n\tPrint current working directory path.\r\n",
                .exec = ush_buildin_cmd_pwd_callback,
        },
        {
                .name = "cat",
                .description = "print files content",
                .help = "cat: cat [file...]\r\n\tPrint concatenate text files content.\r\n",
                .exec = ush_buildin_cmd_cat_callback,
                .process = ush_buildin_cmd_cat_service,
        },
        {
                .name = "xxd",
                .description = "dump file hex content",
                .help = "xxd: xxd [file]\r\n\tShow file hex content.\r\n",
                .exec = ush_buildin_cmd_xxd_callback,
                .process = ush_buildin_cmd_xxd_service,
        }
};

const size_t g_ush_buildin_commands_num = sizeof(g_ush_buildin_commands) / sizeof(g_ush_buildin_commands[0]);
