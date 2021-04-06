#include "ush_internal.h"
#include "ush.h"
#include "ush.h"

#include <string.h>

const char* ush_message_get(struct ush_object *self, ush_message_t msg)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(msg < USH_MESSAGE__TOTAL_NUM);

        switch (msg) {
        case USH_MESSAGE_ERROR_UNKNOWN_COMMAND:
                return "error: unknown command\r\n";
        default:
                return "unknown message\r\n";
        }
}
