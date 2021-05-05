#include "inc/ush.h"

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_CD == 1

void ush_buildin_cmd_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        if (argc != 2) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        ush_status_t status = ush_node_set_current_dir(self, argv[1]);
        if (status != USH_STATUS_OK) {
                ush_print_status(self, status);
                return;
        }
}

#endif /* USH_CONFIG_ENABLE_COMMAND_CD */

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
