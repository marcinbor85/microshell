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

        path_obj->parent = ush_path_by_mount_point(self, mount_point);
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

size_t ush_path_get_levels_count(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        size_t count = 0;
        char ch;
        while (*path) {
                ch = *path;
                if (ch == '/')
                        count++;
                path++;
        }

        return count;
}

void ush_path_get_collapse(struct ush_object *self, char *in_path, char *path, size_t max_len)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(path != NULL);
        USH_ASSERT(max_len > 0);

        strncpy(path, in_path, max_len);

        size_t levels = ush_path_get_levels_count(self, in_path);

        char *nodes[levels];
        size_t index = 0;
        int state = 0;
        while (1) {
                char ch = *in_path;
                if (state == 0) {
                        if (ch == '\0')
                                break;

                        if (ch == '/') {
                                *in_path = '\0';
                                state = 1;
                        }
                } else {
                        if (ch == '\0') {
                                nodes[index++] = in_path;
                                break;
                        }
                        nodes[index++] = in_path;
                        state = 0;
                }
                in_path++;
        }

        char *nodes_out[levels];
        size_t count = 0;
        for (size_t i = 0; i < index; i++) {
                if (strcmp(nodes[i], "..") == 0) {
                        if (count > 0)
                                count--;
                        continue;
                }
                nodes_out[count++] = nodes[i];
        }

        if (count == 0) {
                strncpy(path, "/", max_len);
                return;
        }

        path[0] = '\0';
        for (size_t i = 0; i < count; i++) {
                strncat(path, "/", strlen(path) - max_len);
                strncat(path, nodes_out[i], strlen(path) - max_len);
        }
}

void ush_path_get_absolute_path(struct ush_object *self, const char *in_path, char *path, size_t max_len)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(path != NULL);
        USH_ASSERT(max_len > 0);

        char full_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];
        char abs_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 3];

        if (in_path[0] == '/') {
                strncpy(abs_path, in_path, sizeof(abs_path));
        } else {
                ush_path_get_full_path(self, self->current_path, full_path, sizeof(full_path));
                if (strcmp(full_path, "/") == 0) {
                        snprintf(abs_path, sizeof(abs_path), "/%s", in_path);
                } else {
                        snprintf(abs_path, sizeof(abs_path), "%s/%s", full_path, in_path);
                }
        }

        size_t cd_full_path_len = strlen(abs_path);
        if ((cd_full_path_len > 1) && (abs_path[cd_full_path_len - 1] == '/'))
                abs_path[cd_full_path_len - 1] = '\0';

        ush_path_get_collapse(self, abs_path, path, max_len);
}

bool ush_path_set_current_dir(struct ush_object *self, const char *path)
{
        USH_ASSERT(self != NULL);
        USH_ASSERT(path != NULL);

        char cd_full_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];

        ush_path_get_absolute_path(self, path, cd_full_path, sizeof(cd_full_path));

        bool success = false;

        if (path[0] == '\0')
                return false;

        struct ush_path_object *path_obj = ush_path_by_mount_point(self, cd_full_path);
        if (path_obj != NULL) {
                self->current_path = path_obj;
                success = true;
        }

        return success;
}

void ush_path_get_current_dir(struct ush_object *self, char *path, size_t max_len)
{
        ush_path_get_full_path(self, self->current_path, path, max_len);
}

struct ush_path_object* ush_path_by_mount_point(struct ush_object *self, const char *mount_point)
{
        USH_ASSERT(self != NULL);

        char path_full_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];

        struct ush_path_object *curr = self->path_first;

        while (curr != NULL) {
                ush_path_get_full_path(self, curr, path_full_path, sizeof(path_full_path));

                if ((mount_point != NULL) && (strcmp(path_full_path, mount_point) == 0))
                        return curr;

                if ((mount_point == NULL) && (curr->mount_point == NULL))
                        return curr;

                curr = curr->next;
        }

        return NULL;
}
