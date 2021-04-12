#include "ush.h"
#include "ush_internal.h"

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
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_EXISTS);
                        return;
                }
                if (f->data == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NO_DATA);
                        return;
                }
        }

        ush_process_start(self, file);
}

bool ush_buildin_cmd_cat_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {

        case USH_STATE_PROCESS_START:
                self->process_index = 1;
                self->state = USH_STATE_PROCESS_SERVICE;
                break;

        case USH_STATE_PROCESS_SERVICE: {
                if (self->process_index >= self->args_count) {
                        self->state = USH_STATE_PROCESS_FINISH;
                        break;
                }

                char *argv[self->args_count];
                ush_parse_get_args(self, argv);

                struct ush_file_descriptor const *f = ush_file_find_by_name(self, argv[self->process_index]);

                ush_write_pointer(self, f->data, USH_STATE_PROCESS_SERVICE);
                self->process_index++;
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
