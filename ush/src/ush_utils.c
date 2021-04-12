#include "ush_utils.h"
#include "ush_config.h"
#include "ush_types.h"

#include <string.h>

void ush_utils_join_path(const char *in_path, const char *name, char *out_path)
{
        USH_ASSERT(in_path != NULL);

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
        
        if (*path == '\0')
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

char* ush_utils_get_last_arg(const char *input)
{
        USH_ASSERT(input != NULL);

        size_t len = strlen(input);

        if (len == 0)
                return (char*)input;
        
        len--;
        if (input[len] == ' ')
                return (char*)&input[len + 1];

        while (len > 0) {
                char ch = input[len];
                if (ch == ' ')
                        return (char*)&input[len + 1];
                len--;
        }

        return (char*)input;
}

bool ush_utils_startswith(char *input, char *prefix)
{       
        return (strncmp(prefix, input, strlen(prefix)) == 0) ? true : false;
}

const char* ush_utils_get_status_string(ush_status_t status)
{
        USH_ASSERT(status < USH_STATUS__TOTAL_NUM);

        switch (status) {
        case USH_STATUS_OK:
                return "ok";
        case USH_STATUS_ERROR_NODE_NOT_EXISTS:
                return "error: directory not exist";
        case USH_STATUS_ERROR_NODE_WITH_CHILDS:
                return "error: nested directories exist";
        case USH_STATUS_ERROR_NODE_WITHOUT_PARENT:
                return "error: cannot find parent node";
        case USH_STATUS_ERROR_NODE_ALREADY_MOUNTED:
                return "error: directory already mounted";
        case USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR:
                return "error: syntax error";
        case USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS:
                return "error: wrong arguments";
        case USH_STATUS_ERROR_COMMAND_NOT_EXECUTABLE:
                return "error: command not executable";
        case USH_STATUS_ERROR_FILE_NO_HELP:
                return "error: no help available";
        case USH_STATUS_ERROR_FILE_NOT_EXISTS:
                return "error: file not exists";
        case USH_STATUS_ERROR_FILE_NO_DATA:
                return "error: no data available";
        default:
                USH_ASSERT(false);
        }

        return "error";
}

bool ush_utils_is_printable(uint8_t ch)
{
        return ((ch >= 0x20) && (ch <= 0x7E));
}
