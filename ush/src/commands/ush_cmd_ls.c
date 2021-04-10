#include "ush.h"

void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)argc;
        (void)argv;
        (void)file;

        if (argc > 2) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        // static char buf[512];
        // memset(buf, 0, sizeof(buf));

        // char path_dir[512];
        // struct ush_node_object *path_obj;

        // if (argc == 1) {
        //         ush_path_get_current_dir(self, path_dir, sizeof(path_dir));
        //         path_obj = self->current_node;
        // } else {
        //         ush_path_get_absolute_path(self, argv[1], path_dir, sizeof(path_dir));

        //         path_obj = ush_path_by_mount_point(self, path_dir);
        //         if (path_obj == NULL)
        //                 return (char*)ush_message_get_string(self, USH_MESSAGE_ERROR_DIRECTORY_NOT_FOUND);

        //         ush_path_get_full_path(self, path_obj, path_dir, sizeof(path_dir));
        // }

        // struct ush_node_object *curr = self->root;

        // strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        // strcat(buf, ".");
        // strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

        // if (path_obj->name[0] != '\0') {
        //         strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        //         strcat(buf, "..");
        //         strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");
        // }

        // /* first - directories only in current path */
        // while (curr != NULL) {
        //         if ((curr == path_obj) || (curr->path == NULL) || (strcmp(curr->path, path_dir) != 0)) {
        //                 curr = curr->next;
        //                 continue;
        //         }

        //         strcat(buf, USH_SHELL_FONT_COLOR_GREEN);
        //         strcat(buf, curr->name);
        //         strcat(buf, USH_SHELL_FONT_STYLE_RESET "\r\n");

        //         curr = curr->next;
        // }

        // /* next - files in current path */
        // for (size_t i = 0; i < path_obj->file_list_size; i++) {
        //         struct ush_file_descriptor const *file = &path_obj->file_list[i];
        //         strcat(buf, file->name);
        //         if (file->description != NULL) {
        //                 strcat(buf, "\t- ");
        //                 strcat(buf, file->description);
        //         }
        //         strcat(buf, "\r\n");
        // }
}
