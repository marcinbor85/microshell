#include "ush_types.h"
#include "ush.h"

#include <string.h>

void ush_utils_join_path(const char *in_path, const char *name, char *out_path)
{
        strcpy(out_path, in_path);
        
        if (strcmp(out_path, "/") == 0) {
                strcat(out_path, name);
        } else {
                strcat(out_path, "/");
                strcat(out_path, name);
        }
}

void ush_utils_get_collapse_path(char *in_path, char *out_path)
{
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(out_path != NULL);

        strcpy(out_path, in_path);

        if (strcmp(in_path, "/") == 0)
                return;

        size_t levels = ush_utils_get_path_levels_count(in_path);

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
                } else if (strcmp(nodes[i], ".") == 0) {
                        continue;
                }
                nodes_out[count++] = nodes[i];
        }

        out_path[0] = '\0';
        if (count == 0) {
                strcat(out_path, "/");
                return;
        }

        for (size_t i = 0; i < count; i++) {
                strcat(out_path, "/");
                strcat(out_path, nodes_out[i]);
        }
}

void ush_utils_get_path_level(size_t level, const char *in_path, char *out_path)
{
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(out_path != NULL);
        
        if (level == 0) {
                strcpy(out_path, "/");
                return;
        }

        size_t i = 1;
        while ((i <= level) && (*in_path != '\0')) {
                *out_path = *in_path;
                out_path++;
                in_path++;
                if (*in_path == '/')
                        i++;
        }
        *out_path = '\0';
}

size_t ush_utils_get_path_levels_count(const char *path)
{
        USH_ASSERT(path != NULL);

        if (strcmp(path, "/") == 0)
                return 0;

        size_t count = 1;
        while (*path) {
                path++;
                if (*path == '/')
                        count++;
        }

        return count;
}

void ush_utils_path_upper(char *path)
{
        USH_ASSERT(path != NULL);

        size_t len = strlen(path);
        
        if (len == 1) {
                path[0] = '\0';
                return;
        }

        while (len > 0) {
                len--;
                if (path[len] != '/')
                        continue;

                if (len == 0) {
                        path[len + 1] = '\0';
                } else {
                        path[len] = '\0';
                }
                break;
        }
}

void ush_utils_path_last(const char *in_path, char* *out_path)
{
        USH_ASSERT(in_path != NULL);
        USH_ASSERT(out_path != NULL);

        size_t len = strlen(in_path);
        size_t i = len;

        *out_path = (char*)in_path;

        if (len == 1) 
                return;

        while (i > 0) {
                i--;
                if (in_path[i] != '/')
                        continue;

                *out_path = (char*)&in_path[i + 1];
                break;
        }
}
