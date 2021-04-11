#include "ush.h"
#include "ush_internal.h"

#include <string.h>

void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        switch (argc) {
        case 1:
                self->process_node = self->commands;
                self->process_index = 0;
                self->process_index_item = 0;
                ush_process_start(self, file);
                break;
        case 2: {
                struct ush_file_descriptor const *f = ush_file_find_by_name(self, argv[1]);
                if (f == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                        return;
                }
                if (f->help == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WITHOUT_HELP);
                        return;
                }
                ush_print_no_newline(self, (char*)f->help);
                break;
        }
        default:
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                break;
        }
}

bool ush_buildin_cmd_help_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PROCESS_START:
                if (self->process_node == NULL) {
                        self->state = USH_STATE_RESET_PROMPT;
                        break;
                }

                if (self->process_index_item >= self->process_node->file_list_size) {
                        self->process_index = 0;
                        self->process_index_item = 0;
                        self->process_node = self->process_node->next;
                        break;
                }

                struct ush_file_descriptor const *file = &self->process_node->file_list[self->process_index_item];
                switch (self->process_index) {
                case 0:        
                        ush_write_pointer(self, (char*)file->name, USH_STATE_PROCESS_START);
                        self->process_index = 1;
                        break;
                case 1:
                        if (file->description != NULL) {
                                ush_write_pointer(self, "\t- ", USH_STATE_PROCESS_START);
                                self->process_index = 2;
                                break;
                        }
                        self->process_index = 3;
                        break;
                case 2:
                        ush_write_pointer(self, (char*)file->description, USH_STATE_PROCESS_START);
                        self->process_index = 3;
                        break;
                case 3:
                        ush_write_pointer(self, "\r\n", USH_STATE_PROCESS_START);
                        self->process_index = 0;
                        self->process_index_item++;
                        break;
                }
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
