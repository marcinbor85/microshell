/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
                        if (f->description != NULL) {
                                sprintf(self->desc->output_buffer, "%-" USH_STRING(USH_CONFIG_FILENAME_ALIGN_SPACE) "s " USH_SHELL_FONT_COLOR_YELLOW "- " , (char*)f->name);
                                self->process_index = 1;
                        } else {
                                sprintf(self->desc->output_buffer, "%s", (char*)f->name);
                                self->process_index = 2;
                        }
                        ush_write_pointer(self, self->desc->output_buffer, self->state);
                        break;
                case 1:
                        ush_write_pointer(self, (char*)f->description, self->state);
                        self->process_index = 2;
                        break;
                case 2:
                        if (f->description != NULL) {
                                ush_write_pointer(self, USH_SHELL_FONT_STYLE_RESET "\r\n", self->state);
                        } else {
                                ush_write_pointer(self, "\r\n", self->state);
                        }
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
