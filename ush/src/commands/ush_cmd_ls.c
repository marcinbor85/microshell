#include "ush.h"

#include <string.h>

void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        if (argc > 2) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        static char buf[512];
        memset(buf, 0, sizeof(buf));

        char abs_path[USH_CONFIG_PATH_MAX_LENGTH];
        struct ush_node_object *node;
        
        if (argc == 1) {
                node = self->current_node;
        } else {
                ush_node_get_absolute_path(self, argv[1], abs_path);

                node = ush_node_get_by_path(self, abs_path);
                if (node == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_NODE_NOT_EXISTS);
                        return;
                }
        }

        strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        strcat(buf, ".");
        strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

        if (node->parent != NULL) {
                strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
                strcat(buf, "..");
                strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");
        }

        struct ush_node_object *curr = node->childs;
        while (curr != NULL) {
                char *name;
                ush_utils_path_last(curr->path, &name);

                strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
                strcat(buf, name);
                strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

                curr = curr->next;
        }

        for (size_t i = 0; i < node->file_list_size; i++) {
                struct ush_file_descriptor const *file = &node->file_list[i];
                strcat(buf, file->name);
                if (file->description != NULL) {
                        strcat(buf, "\t- ");
                        strcat(buf, file->description);
                }
                strcat(buf, "\r\n");
        }

        ush_print_no_newline(self, buf);
}
