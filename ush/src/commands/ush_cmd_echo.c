#include "ush.h"
#include "ush_internal.h"

#include <string.h>

#if USH_CONFIG_ENABLE_FEATURE_COMMANDS == 1

#if USH_CONFIG_ENABLE_COMMAND_ECHO == 1

void ush_buildin_cmd_echo_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)argv;
        (void)file;

        struct ush_file_descriptor const *f = NULL;
        char *data = "";

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
                        ush_print_status(self, USH_STATUS_ERROR_FILE_READ_ONLY);
                        break;
                }
                /* fallthrough */
        case 2:
                data = argv[1];
                /* fallthrough */
        case 1:
                if (f != NULL) {
                        f->set_data(self, f, (uint8_t*)data, strlen(data));
                } else {
                        ush_print(self, data);
                }
                break;
        default:
                ush_print_status(self, USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS);
                break;
        }
}

#endif /* USH_CONFIG_ENABLE_COMMAND_ECHO */

#endif /* USH_CONFIG_ENABLE_FEATURE_COMMANDS */
