#include "ush_internal.h"
#include "ush_config.h"
#include "ush_file.h"
#include "ush.h"

#include <string.h>

void ush_parse_finish(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char *argv[self->args_count];
        int argc;

        argc = ush_parse_get_args(self, argv);
        if (argc == 0)
                return;
        
        if (self->desc->exec != NULL)
                self->desc->exec(self, NULL, argc, argv);

        struct ush_file_descriptor const *file = ush_file_find_by_name(self, argv[0]);
        if (file == NULL) {
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR);
                return;
        }

        if (file->exec == NULL) {
                ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_EXECUTABLE);
                return;
        }
        
        file->exec(self, file, argc, argv);
}

bool ush_parse_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool processed = true;

        switch (self->state) {
        case USH_STATE_PARSE_PREPARE:
                ush_parse_start(self);
                break;
        case USH_STATE_PARSE_SEARCH_ARG:
        case USH_STATE_PARSE_QUOTE_ARG:
        case USH_STATE_PARSE_STANDARD_ARG:
        case USH_STATE_PARSE_SEARCH_STOP:
                ush_parse_char(self);
                break;
        default:
                processed = false;
                break;
        }

        return processed;
}
