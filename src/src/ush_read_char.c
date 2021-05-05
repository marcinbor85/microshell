#include "inc/ush_internal.h"
#include "inc/ush_preconfig.h"

bool ush_read_char(struct ush_object *self)
{
        USH_ASSERT(self != NULL);

        char ch;
        bool echo = true;

        if (self->desc->io->read(self, &ch) == 0)
                return false;
        
        switch (ch) {
        case '\x03':
                /* ctrl+c */
                ush_write_pointer(self, "^C", USH_STATE_RESET);
                echo = false;
                break;
        case '\x08':
        case '\x7F':
                /* backspace */
                if (self->in_pos > 0) {
                        self->in_pos--;
                        self->desc->input_buffer[self->in_pos] = '\0';
                } else {
                        echo = false;
                }
                break;
        case '\x09':
                /* tab */
#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1      
                ush_autocomp_start(self);
#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
                echo = false;
                break;
        case '\x1B':
                /* escape */
                self->ansi_escape_state = 1;
                echo = false;
                break;
        default:
                echo = ush_read_char_by_escape_state(self, ch);
                break;
        }

        if (echo != false)
                ush_read_echo_service(self, ch);
        
        return true;
}
