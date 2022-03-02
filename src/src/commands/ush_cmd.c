/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "inc/ush_types.h"
#include "inc/ush_preconfig.h"

#ifdef USH_CONFIG_ENABLE_FEATURE_COMMANDS

#ifdef USH_CONFIG_ENABLE_COMMAND_HELP
extern void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_help_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_HELP */

#ifdef USH_CONFIG_ENABLE_COMMAND_LS
extern void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_ls_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_LS */

#ifdef USH_CONFIG_ENABLE_COMMAND_CD
extern void ush_buildin_cmd_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
#endif /* USH_CONFIG_ENABLE_COMMAND_CD */

#ifdef USH_CONFIG_ENABLE_COMMAND_PWD
extern void ush_buildin_cmd_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
#endif /* USH_CONFIG_ENABLE_COMMAND_PWD */

#ifdef USH_CONFIG_ENABLE_COMMAND_CAT
extern void ush_buildin_cmd_cat_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_cat_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_CAT */

#ifdef USH_CONFIG_ENABLE_COMMAND_XXD
extern void ush_buildin_cmd_xxd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern void ush_buildin_cmd_xxd_service(struct ush_object *self, struct ush_file_descriptor const *file);
#endif /* USH_CONFIG_ENABLE_COMMAND_XXD */

#ifdef USH_CONFIG_ENABLE_COMMAND_ECHO
extern void ush_buildin_cmd_echo_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */

const struct ush_file_descriptor g_ush_buildin_commands[] = {
#ifdef USH_CONFIG_ENABLE_COMMAND_HELP
        {
                .name = "help",
                .description = "list available commands",
                .help = "usage: help [file]\r\n",
                .exec = ush_buildin_cmd_help_callback,
                .process = ush_buildin_cmd_help_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_HELP */
#ifdef USH_CONFIG_ENABLE_COMMAND_LS
        {
                .name = "ls",
                .description = "list directory content",
                .help = "usage: ls [path]\r\n",
                .exec = ush_buildin_cmd_ls_callback,
                .process = ush_buildin_cmd_ls_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_LS */
#ifdef USH_CONFIG_ENABLE_COMMAND_CD
        {
                .name = "cd",
                .description = "change current directory",
                .help = "usage: cd <path>\r\n",
                .exec = ush_buildin_cmd_cd_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_CD */
#ifdef USH_CONFIG_ENABLE_COMMAND_PWD
        {
                .name = "pwd",
                .description = "print current directory",
                .help = "usage: pwd\r\n",
                .exec = ush_buildin_cmd_pwd_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_PWD */
#ifdef USH_CONFIG_ENABLE_COMMAND_CAT
        {
                .name = "cat",
                .description = "print files content",
                .help = "usage: cat <files...>\r\n",
                .exec = ush_buildin_cmd_cat_callback,
                .process = ush_buildin_cmd_cat_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_CAT */
#ifdef USH_CONFIG_ENABLE_COMMAND_XXD
        {
                .name = "xxd",
                .description = "dump file hex content",
                .help = "usage: xxd <file>\r\n",
                .exec = ush_buildin_cmd_xxd_callback,
                .process = ush_buildin_cmd_xxd_service,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_XXD */
#ifdef USH_CONFIG_ENABLE_COMMAND_ECHO
        {
                .name = "echo",
                .description = "print string to file",
                .help = "usage: echo <string> > <file>\r\n"
                        "       echo [string]\r\n"
                        "       * can be ascii-hex encoded string with \\xNN format.\r\n",
                .exec = ush_buildin_cmd_echo_callback,
        },
#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */
};

const size_t g_ush_buildin_commands_num = sizeof(g_ush_buildin_commands) / sizeof(g_ush_buildin_commands[0]);

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
