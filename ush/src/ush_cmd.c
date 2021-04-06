#include "ush_internal.h"
#include "ush.h"

#include <string.h>

void ush_cmd_register(struct ush_object *self, struct ush_cmd_object *cmd, const struct ush_cmd_descriptor *desc)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(cmd != NULL);
        USH_ASSERT(desc != NULL);

        cmd->desc = desc;
        cmd->next = self->cmd_first;
        self->cmd_first = cmd;
}

void ush_cmd_unregister(struct ush_object *self, struct ush_cmd_object *cmd)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(cmd != NULL);

        struct ush_cmd_object *prev = NULL;
        struct ush_cmd_object *curr = self->cmd_first;

        while (curr != NULL) {
                if (curr == cmd) {
                        if (prev == NULL) {
                                self->cmd_first = cmd->next;
                        } else {
                                prev->next = cmd->next;
                        }
                        break;
                }
                prev = curr;
                curr = curr->next;
        }
}

struct ush_cmd_object* ush_cmd_find_by_name(struct ush_object *self, char *name)
{
        struct ush_cmd_object *curr = self->cmd_first;
        
        while (curr != NULL) {
                if (strcmp(curr->desc->name, name) == 0)
                        return curr;
                curr = curr->next;
        }

        return NULL;
}
