#include "inc/ush_types.h"
#include "inc/ush_preconfig.h"

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
                .help = "usage: help [file]\r\n",
                .exec = ush_buildin_cmd_help_callback,
                .process = ush_buildin_cmd_help_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_HELP */
#if USH_CONFIG_ENABLE_COMMAND_LS == 1
        {
                .name = "ls",
                .description = "list directory content",
                .help = "usage: ls [path]\r\n",
                .exec = ush_buildin_cmd_ls_callback,
                .process = ush_buildin_cmd_ls_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_LS */
#if USH_CONFIG_ENABLE_COMMAND_CD == 1
        {
                .name = "cd",
                .description = "change current directory",
                .help = "usage: cd <path>\r\n",
                .exec = ush_buildin_cmd_cd_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_CD */
#if USH_CONFIG_ENABLE_COMMAND_PWD == 1
        {
                .name = "pwd",
                .description = "print current directory",
                .help = "usage: pwd\r\n",
                .exec = ush_buildin_cmd_pwd_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_PWD */
#if USH_CONFIG_ENABLE_COMMAND_CAT == 1
        {
                .name = "cat",
                .description = "print files content",
                .help = "usage: cat <files...>\r\n",
                .exec = ush_buildin_cmd_cat_callback,
                .process = ush_buildin_cmd_cat_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_CAT */
#if USH_CONFIG_ENABLE_COMMAND_XXD == 1
        {
                .name = "xxd",
                .description = "dump file hex content",
                .help = "usage: xxd <file>\r\n",
                .exec = ush_buildin_cmd_xxd_callback,
                .process = ush_buildin_cmd_xxd_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_XXD */
#if USH_CONFIG_ENABLE_COMMAND_ECHO == 1
        {
                .name = "echo",
                .description = "print string to file",
                .help = "usage: echo [string] > [file]\r\n"
                        "       echo [string]\r\n"
                        "       * can be ascii-hex encoded string with \\xNN format.\r\n",
                .exec = ush_buildin_cmd_echo_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */
};

const size_t g_ush_buildin_commands_num = sizeof(g_ush_buildin_commands) / sizeof(g_ush_buildin_commands[0]);

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
