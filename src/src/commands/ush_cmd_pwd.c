#include "inc/ush.h"

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_PWD == 1

void ush_buildin_cmd_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argv;
        (void)file;

        if (argc != 1) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        ush_print(self, (char*)self->current_node->path);
}

#endif /* USH_CONFIG_ENABLE_COMMAND_PWD */

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
