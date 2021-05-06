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

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_CAT == 1

void ush_buildin_cmd_cat_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        if (argc < 2) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                return;
        }

        for (int i = 1; i < argc; i++) {
                struct ush_file_descriptor const *f = ush_file_find_by_name(self, argv[i]);
                if (f == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_FOUND);
                        return;
                }
                if (f->get_data == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_READABLE);
                        return;
                }
        }

        ush_process_start(self, file);
}

bool ush_buildin_cmd_cat_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        (void)file;
        
        USH_ASSERT(self != NULL);
        USH_ASSERT(file != NULL);

        bool processed = true;

        switch (self->state) {

        case USH_STATE_PROCESS_START:
                self->process_index_item = 1;
                self->state = USH_STATE_PROCESS_SERVICE;
                break;

        case USH_STATE_PROCESS_SERVICE: {
                if (self->process_index_item >= self->args_count) {
                        self->state = USH_STATE_PROCESS_FINISH;
                        break;
                }

                char *argv[self->args_count];
                ush_parse_get_args(self, argv);

                struct ush_file_descriptor const *f = ush_file_find_by_name(self, argv[self->process_index_item]);

                uint8_t *data;
                size_t data_size = f->get_data(self, f, &data);

                ush_write_pointer_bin(self, data, data_size, self->state);
                self->process_index_item++;
                break;
        }

        case USH_STATE_PROCESS_FINISH:
                self->state = USH_STATE_RESET_PROMPT;
                break;

        default:
                processed = false;
                break;
        }

        return processed;
}

#endif /* USH_CONFIG_ENABLE_COMMAND_CAT */

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
