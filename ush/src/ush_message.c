#include "ush_internal.h"
#include "ush.h"
#include "ush.h"

#include <string.h>

const char* ush_message_get_string(struct ush_object *self, ush_message_t msg)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(msg < USH_MESSAGE__TOTAL_NUM);

        switch (msg) {
        case USH_MESSAGE_ERROR_UNKNOWN_COMMAND:
                return "error: unknown command\r\n";
        case USH_MESSAGE_ERROR_WRONG_ARGUMENTS:
                return "error: wrong arguments\r\n";
        case USH_MESSAGE_ERROR_DIRECTORY_NOT_FOUND:
                return "error: directory not found\r\n";
        case USH_MESSAGE_ERROR_COMMAND_NOT_FOUND:
                return "error: command not found\r\n";
        case USH_MESSAGE_ERROR_NO_HELP_AVAILABLE:
                return "error: no help available\r\n";
        case USH_MESSAGE_ERROR_CANNOT_EXECUTE:
                return "error: cannot execute\r\n";
        default:
                return "unknown message\r\n";
        }
}
