#include "ush_internal.h"
#include "ush.h"

#include <string.h>

void ush_path_mount(struct ush_object *self, const char *mount_point, const char *name, struct ush_path_object *path_obj, const struct ush_cmd_descriptor *cmd_list, size_t cmd_list_size)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path_obj != NULL);
        USH_ASSERT(cmd_list != NULL);
        USH_ASSERT(cmd_list_size > 0);

        for (size_t i = 0; i < cmd_list_size; i++) {
                USH_ASSERT(cmd_list[i].name != NULL);
        }

        path_obj->cmd_list = cmd_list;
        path_obj->cmd_list_size = cmd_list_size;
        path_obj->mount_point = mount_point;
        path_obj->name = name;
        path_obj->next = self->path_first;
        self->path_first = path_obj;
}

void ush_path_unmount(struct ush_object *self, struct ush_path_object *path_obj)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path_obj != NULL);

        struct ush_path_object *prev = NULL;
        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                if (curr != path_obj) {
                        prev = curr;
                        curr = curr->next;
                        continue;
                }
                if (prev == NULL) {
                        self->path_first = path_obj->next;
                } else {
                        prev->next = path_obj->next;
                }
                break;                
        }
}

void ush_path_get_full_path(struct ush_object *self, struct ush_path_object *path_obj, char *full_path, size_t max_len)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path_obj != NULL);
        USH_ASSERT(full_path != NULL);
        USH_ASSERT(max_len > 0);

        if (path_obj->mount_point != NULL) {
                if (strcmp(path_obj->mount_point, "/") == 0) {
                        snprintf(full_path, max_len, "/%s", path_obj->name);
                } else {
                        snprintf(full_path, max_len, "%s/%s", path_obj->mount_point, path_obj->name);
                }       
        } else {
                full_path[0] = '\0';
        }
}

bool ush_path_set_current_dir(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char cd_full_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 3];
        char path_full_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];

        bool success = false;

        if (path[0] == '\0')
                return false;

        if (path[0] == '/') {
                strncpy(cd_full_path, path, sizeof(cd_full_path));
        } else {
                ush_path_get_full_path(self, self->current_path, path_full_path, sizeof(path_full_path));
                if (strcmp(path_full_path, "/") == 0) {
                        snprintf(cd_full_path, sizeof(cd_full_path), "/%s", path);
                } else {
                        snprintf(cd_full_path, sizeof(cd_full_path), "%s/%s", path_full_path, path);
                }
        }

        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                if (curr->mount_point == NULL) {
                        curr = curr->next;
                        continue;
                }

                ush_path_get_full_path(self, curr, path_full_path, sizeof(path_full_path));

                if (strcmp(path_full_path, cd_full_path) != 0) {
                        curr = curr->next;
                        continue;
                }

                self->current_path = curr;
                success = true;
                break;
        }

        return success;
}

void ush_path_get_current_dir(struct ush_object *self, char *path, size_t max_len)
{
        ush_path_get_full_path(self, self->current_path, path, max_len);
}