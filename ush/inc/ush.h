#ifndef USH_H
#define USH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"
#include "ush_config.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc);
bool ush_service(struct ush_object *self);
void ush_reset(struct ush_object *self);
void ush_cmd_register(struct ush_object *self, struct ush_cmd_object *cmd, const struct ush_cmd_descriptor *desc);
void ush_cmd_unregister(struct ush_object *self, struct ush_cmd_object *cmd);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
