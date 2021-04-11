#include "ush.h"
#include "ush_internal.h"

#include <string.h>

void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        if (argc > 2) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        char abs_path[USH_CONFIG_PATH_MAX_LENGTH];

        if (argc == 1) {
                self->process_node = self->current_node;
        } else {
                ush_node_get_absolute_path(self, argv[1], abs_path);

                self->process_node = ush_node_get_by_path(self, abs_path);
                if (self->process_node == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_NODE_NOT_EXISTS);
                        return;
                }
        }
        
        ush_process_start(self, file);
}

bool ush_buildin_cmd_ls_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PROCESS_START:
                if (self->process_node->parent != NULL) {
                        ush_write_pointer(self, USH_SHELL_FONT_COLOR_GREEN ".\r\n..\r\n", USH_STATE_PROCESS_SERVICE);
                } else {
                        ush_write_pointer(self, USH_SHELL_FONT_COLOR_GREEN ".\r\n", USH_STATE_PROCESS_SERVICE);
                }
                self->process_child_node = self->process_node->childs;
                self->process_index = 0;
                break;
        case USH_STATE_PROCESS_SERVICE:
                if (self->process_child_node == NULL) {
                        ush_write_pointer(self, USH_SHELL_FONT_STYLE_RESET, USH_STATE_PROCESS_FINISH);
                        self->process_index = 0;
                        self->process_index_item = 0;
                        break;
                }

                switch (self->process_index) {
                case 0: {
                        char *name;
                        ush_utils_path_last(self->process_child_node->path, &name);
                        ush_write_pointer(self, name, USH_STATE_PROCESS_SERVICE);
                        self->process_index = 1;
                        break;
                }
                case 1:
                        ush_write_pointer(self, "/\r\n", USH_STATE_PROCESS_SERVICE);
                        self->process_index = 2;
                        break;
                case 2:
                        self->process_index = 0;
                        self->process_child_node = self->process_child_node->next;
                        break;
                default:
                        USH_ASSERT(false);
                        break;
                }
                break;
        case USH_STATE_PROCESS_FINISH:
                if (self->process_index_item >= self->process_node->file_list_size) {
                        self->state = USH_STATE_RESET_PROMPT;
                        break;
                }

                struct ush_file_descriptor const *file = &self->process_node->file_list[self->process_index_item];
                switch (self->process_index) {
                case 0:        
                        ush_write_pointer(self, (char*)file->name, USH_STATE_PROCESS_FINISH);
                        self->process_index = 1;
                        break;
                case 1:
                        if (file->description != NULL) {
                                ush_write_pointer(self, "\t- ", USH_STATE_PROCESS_FINISH);
                                self->process_index = 2;
                                break;
                        }
                        self->process_index = 3;
                        break;
                case 2:
                        ush_write_pointer(self, (char*)file->description, USH_STATE_PROCESS_FINISH);
                        self->process_index = 3;
                        break;
                case 3:
                        ush_write_pointer(self, "\r\n", USH_STATE_PROCESS_FINISH);
                        self->process_index = 0;
                        self->process_index_item++;
                        break;
                default:
                        USH_ASSERT(false);
                        break;
                }
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
