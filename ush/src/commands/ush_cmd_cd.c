#include "ush.h"

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
