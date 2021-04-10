#include "ush.h"

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
