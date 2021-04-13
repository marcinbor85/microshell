#include "ush_types.h"
#include "ush_config.h"

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_HELP == 1
extern void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_help_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_HELP */

#if USH_CONFIG_ENABLE_COMMAND_LS == 1
extern void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_ls_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_LS */

#if USH_CONFIG_ENABLE_COMMAND_CD == 1
extern void ush_buildin_cmd_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
#endif /* USH_CONFIG_ENABLE_COMMAND_CD */

#if USH_CONFIG_ENABLE_COMMAND_PWD == 1
extern void ush_buildin_cmd_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
#endif /* USH_CONFIG_ENABLE_COMMAND_PWD */

#if USH_CONFIG_ENABLE_COMMAND_CAT == 1
extern void ush_buildin_cmd_cat_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_cat_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_CAT */

#if USH_CONFIG_ENABLE_COMMAND_XXD == 1
extern void ush_buildin_cmd_xxd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_xxd_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_XXD */

#if USH_CONFIG_ENABLE_COMMAND_ECHO == 1
extern void ush_buildin_cmd_echo_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */

const struct ush_file_descriptor g_ush_buildin_commands[] = {
#if USH_CONFIG_ENABLE_COMMAND_HELP == 1
        {
                .name = "help",
                .description = "list available commands",
                .help = "help: help [file]\r\n\tShow help information for file or command.\r\n"
                        "      help\r\n\tShow available commands.\r\n",
                .exec = ush_buildin_cmd_help_callback,
                .process = ush_buildin_cmd_help_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_HELP */
#if USH_CONFIG_ENABLE_COMMAND_LS == 1
        {
                .name = "ls",
                .description = "list directory content",
                .help = "ls: ls [path]\r\n\tList directory content.\r\n"
                        "    ls\r\n\tList current directory content.\r\n",
                .exec = ush_buildin_cmd_ls_callback,
                .process = ush_buildin_cmd_ls_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_LS */
#if USH_CONFIG_ENABLE_COMMAND_CD == 1
        {
                .name = "cd",
                .description = "change current directory",
                .help = "cd: cd [path]\r\n\tChange current working directory.\r\n",
                .exec = ush_buildin_cmd_cd_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_CD */
#if USH_CONFIG_ENABLE_COMMAND_PWD == 1
        {
                .name = "pwd",
                .description = "print current directory",
                .help = "pwd: pwd\r\n\tPrint current working directory path.\r\n",
                .exec = ush_buildin_cmd_pwd_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_PWD */
#if USH_CONFIG_ENABLE_COMMAND_CAT == 1
        {
                .name = "cat",
                .description = "print files content",
                .help = "cat: cat [file...]\r\n\tPrint concatenate text files content.\r\n",
                .exec = ush_buildin_cmd_cat_callback,
                .process = ush_buildin_cmd_cat_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_CAT */
#if USH_CONFIG_ENABLE_COMMAND_XXD == 1
        {
                .name = "xxd",
                .description = "dump file hex content",
                .help = "xxd: xxd [file]\r\n\tShow file hex content.\r\n",
                .exec = ush_buildin_cmd_xxd_callback,
                .process = ush_buildin_cmd_xxd_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_XXD */
#if USH_CONFIG_ENABLE_COMMAND_ECHO == 1
        {
                .name = "echo",
                .description = "print string to file",
                .help = "echo: echo [string] > [file]\r\n\tPrint string to file.\r\n"
                        "      echo [string] \r\n\tPrint string to console.\r\n",
                .exec = ush_buildin_cmd_echo_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */
};

const size_t g_ush_buildin_commands_num = sizeof(g_ush_buildin_commands) / sizeof(g_ush_buildin_commands[0]);

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
