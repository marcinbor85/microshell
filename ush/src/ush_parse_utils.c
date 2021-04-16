#include "ush_internal.h"
#include "ush_config.h"

#include <string.h>

void ush_parse_start(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        self->in_pos = 0;
        self->out_pos = 0;
        self->args_count = 0;
        self->escape_flag = false;
        self->state = USH_STATE_PARSE_SEARCH_ARG;
}

int ush_parse_get_args(struct ush_object *self, char* *argv)
{
        char *ptr = self->desc->input_buffer;
        int argc = self->args_count;

        if (argv == NULL)
                return argc;

        for (int i = 0; i < argc; i++) {
                argv[i] = ptr;
                ptr += strlen(argv[i]) + 1;
        }

        return argc;        
}
