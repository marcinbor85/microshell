#include "inc/ush.h"
#include "inc/ush_internal.h"

#include <string.h>

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_HELP == 1

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
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_FOUND);
                        return;
                }
                if (f->help == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NO_HELP);
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

bool ush_buildin_cmd_help_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        (void)file;

        USH_ASSERT(self != NULL);
        USH_ASSERT(file != NULL);

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

                struct ush_file_descriptor const *f = &self->process_node->file_list[self->process_index_item];
                switch (self->process_index) {
                case 0:        
                        sprintf(self->desc->output_buffer, "%-" USH_STRING(USH_CONFIG_FILENAME_ALIGN_SPACE) "s", (char*)f->name);
                        ush_write_pointer(self, self->desc->output_buffer, self->state);
                        self->process_index = 1;
                        break;
                case 1:
                        if (f->description != NULL) {
                                ush_write_pointer(self, "- ", self->state);
                                self->process_index = 2;
                                break;
                        }
                        self->process_index = 3;
                        break;
                case 2:
                        ush_write_pointer(self, (char*)f->description, self->state);
                        self->process_index = 3;
                        break;
                case 3:
                        ush_write_pointer(self, "\r\n", self->state);
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

#endif /* USH_CONFIG_ENABLE_COMMAND_HELP */

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
