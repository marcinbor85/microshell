#include "ush_internal.h"
#include "ush.h"

#include <string.h>

void ush_path_register(struct ush_object *self, const char *path, struct ush_path_object *path_obj, const struct ush_cmd_descriptor *cmd_list, size_t cmd_list_size)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path_obj != NULL);

        path_obj->cmd_list = cmd_list;
        path_obj->cmd_list_size = cmd_list_size;
        path_obj->path = path;
        path_obj->next = self->path_first;
        self->path_first = path_obj;
}

void ush_path_unregister(struct ush_object *self, struct ush_path_object *path_obj)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path_obj != NULL);

        struct ush_path_object *prev = NULL;
        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                if (curr == path_obj) {
                        if (prev == NULL) {
                                self->path_first = path_obj->next;
                        } else {
                                prev->next = path_obj->next;
                        }
                        break;
                }
                prev = curr;
                curr = curr->next;
        }
}
