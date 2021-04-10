#include "ush.h"

#include <string.h>

void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        if (argc == 1) {
                struct ush_node_object *curr = self->commands;
                while (curr != NULL) {
                        for (size_t i = 0; i < curr->file_list_size; i++) {
                                struct ush_file_descriptor const *f = &curr->file_list[i];
                                strcat(buf, f->name);
                                if (f->description != NULL) {
                                        strcat(buf, "\t- ");
                                        strcat(buf, f->description);
                                }
                                strcat(buf, "\r\n");
                        }

                        curr = curr->next;
                }
        } else if (argc == 2) {
                struct ush_file_descriptor const *help_cmd = ush_file_find_by_name(self, argv[1]);
                if (help_cmd == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                        return;
                }

                if (help_cmd->help == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WITHOUT_HELP);
                        return;
                }

                strcat(buf, help_cmd->help);
        } else {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        ush_print_no_newline(self, buf);
}
