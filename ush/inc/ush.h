#ifndef USH_H
#define USH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"
#include "ush_config.h"
#include "ush_shell.h"

#include "ush_file.h"
#include "ush_node.h"
#include "ush_utils.h"
#include "ush_commands.h"

void ush_init(struct ush_object *self, const struct ush_descriptor *desc);
bool ush_service(struct ush_object *self);
void ush_reset(struct ush_object *self);

void ush_print_status(struct ush_object *self, ush_status_t status);
void ush_print(struct ush_object *self, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
