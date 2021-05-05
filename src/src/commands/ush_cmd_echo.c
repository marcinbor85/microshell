#include "inc/ush.h"
#include "inc/ush_internal.h"
#include "inc/ush_utils.h"

#include <string.h>

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_ECHO == 1

void ush_buildin_cmd_echo_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argv;
        (void)file;

        struct ush_file_descriptor const *f = NULL;
        uint8_t *data = NULL;
        size_t data_size = 0;

        switch (argc) {
        
        case 4:
                if (strcmp(argv[2], ">") != 0) {
                        ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                        break;
                }
                f = ush_file_find_by_name(self, argv[3]);
                if (f == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_FOUND);
                        break;
                }
                if (f->set_data == NULL) {
                        ush_print_status(self, USH_STATUS_ERROR_FILE_NOT_WRITABLE);
                        break;
                }
                /* fallthrough */
        case 2: {
                char *data_in = argv[1];
                size_t max_size = strlen(data_in);
                data_size = ush_utils_decode_ascii(data_in, (uint8_t*)data_in, max_size);
                data = (uint8_t*)data_in;
        }
                /* fallthrough */
        case 1:
                if (f != NULL) {
                        f->set_data(self, f, data, data_size);
                } else {
                        if (data != NULL) {
                                ush_print(self, (char*)data);
                        } else {
                                self->state = USH_STATE_RESET;
                        }
                }
                break;
        default:
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                break;
        }
}

#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
