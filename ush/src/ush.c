#include "ush.h"
#include "ush_internal.h"
#include "ush_shell.h"
#include "ush_const.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(desc != NULL);

        USH_ASSERT(desc->input_buffer != NULL);
        USH_ASSERT(desc->input_buffer_size > 0);
        USH_ASSERT(desc->output_buffer != NULL);
        USH_ASSERT(desc->output_buffer_size >= 4); /* internal echo buffer */

        USH_ASSERT(desc->io != NULL);
        USH_ASSERT(desc->io->read != NULL);
        USH_ASSERT(desc->io->write != NULL);

        USH_ASSERT(desc->hostname != NULL);

        self->desc = desc;
        self->root = NULL;

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1
        ush_commands_add(self, &self->buildin_commands, g_ush_buildin_commands, g_ush_buildin_commands_num);
#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */

        ush_reset(self);
}

bool ush_service(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        bool busy = false;

        if (ush_reset_service(self) != false)
                return true;        
        if (ush_prompt_service(self) != false)
                return true;        
        if (ush_read_service(self, &busy) != false)
                return busy;
#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1
        if (ush_autocomp_service(self) != false)
                return true;
#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
        if (ush_parse_service(self) != false)
                return true;        
        if (ush_write_service(self) != false)
                return true;
        if (ush_process_service(self) != false)
                return true;
        
        USH_ASSERT(false);
        
        return false;
}

void ush_reset(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        ush_write_pointer(self, USH_NAME " " USH_VERSION "\r\n", USH_STATE_RESET_PROMPT);
}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        char *ret = (char*)ush_utils_get_status_string(status);
        ush_write_pointer(self, ret, USH_STATE_RESET);
}

void ush_print(struct ush_object *self, char *buf)
{
        ush_write_pointer(self, buf, USH_STATE_RESET);
}

void ush_print_no_newline(struct ush_object *self, char *buf)
{
        ush_write_pointer(self, buf, USH_STATE_RESET_PROMPT);
}
