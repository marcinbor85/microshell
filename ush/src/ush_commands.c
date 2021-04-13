#include "ush_commands.h"
#include "ush_types.h"
#include "ush_config.h"

ush_status_t ush_commands_add(struct ush_object *self, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(node != NULL);
        USH_ASSERT(file_list != NULL);
        USH_ASSERT(file_list_size > 0);

        for (size_t i = 0; i < file_list_size; i++) {
                USH_ASSERT(file_list[i].name != NULL);
        }

        node->file_list = file_list;
        node->file_list_size = file_list_size;
        node->path = NULL;

        node->next = self->commands;
        self->commands = node;

        return USH_STATUS_OK;
}

ush_status_t ush_commands_remove(struct ush_object *self, struct ush_node_object *node)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(node != NULL);

        struct ush_node_object *prev = NULL;
        struct ush_node_object *curr = self->commands;

        while (curr != NULL) {
                if (curr != node) {
                        prev = curr;
                        curr = curr->next;
                        continue;
                }
                if (prev == NULL) {
                        self->commands = node->next;
                } else {
                        prev->next = node->next;
                }
                return USH_STATUS_OK;           
        }

        return USH_STATUS_ERROR_NODE_NOT_FOUND;
}
