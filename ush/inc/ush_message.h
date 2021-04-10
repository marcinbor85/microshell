#ifndef USH_MESSAGE_H
#define USH_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

const char* ush_message_get_string(struct ush_object *self, ush_message_t msg);

#ifdef __cplusplus
}
#endif

#endif /* USH_MESSAGE_H */
