/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "inc/ush_utils.h"
#include "inc/ush_preconfig.h"
#include "inc/ush_types.h"

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

char* ush_utils_path_last(const char *in_path)
{
        USH_ASSERT(in_path != NULL);

        size_t len = strlen(in_path);
        size_t i = len;

        char *out = (char*)in_path;

        if (len == 1) 
                return out;

        while (i > 0) {
                i--;
                if (in_path[i] != '/')
                        continue;

                out = (char*)&in_path[i + 1];
                break;
        }

        return out;
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

bool ush_utils_startswith(const char *input, char *prefix)
{       
        return (strncmp(prefix, input, strlen(prefix)) == 0) ? true : false;
}

const char* ush_utils_get_status_string(ush_status_t status)
{
        USH_ASSERT(status < USH_STATUS__TOTAL_NUM);

        switch (status) {
        case USH_STATUS_OK:
                return USH_CONFIG_TRANSLATION_OK;
        case USH_STATUS_ERROR_NODE_NOT_FOUND:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_DIRECTORY_NOT_FOUND;
        case USH_STATUS_ERROR_NODE_WITH_CHILDS:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_NESTED_DIRECTORIES_EXIST;
        case USH_STATUS_ERROR_NODE_WITHOUT_PARENT:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_CANNOT_FIND_PARENT_NODE;
        case USH_STATUS_ERROR_NODE_ALREADY_MOUNTED:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_DIRECTORY_ALREADY_MOUNTED;
        case USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_SYNTAX_ERROR;
        case USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_WRONG_ARGUMENTS;
        case USH_STATUS_ERROR_FILE_NOT_EXECUTABLE:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_FILE_NOT_EXECUTABLE;
        case USH_STATUS_ERROR_FILE_NOT_WRITABLE:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_FILE_NOT_WRITABLE;
        case USH_STATUS_ERROR_FILE_NOT_READABLE:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_FILE_NOT_READABLE;
        case USH_STATUS_ERROR_FILE_NO_HELP:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_NO_HELP_AVAILABLE;
        case USH_STATUS_ERROR_FILE_NOT_FOUND:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_FILE_NOT_FOUND;
        case USH_STATUS_ERROR_FILE_READ_ONLY:
                return USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_READ_ONLY_FILE;
        default:
                break;
        }

        return USH_CONFIG_TRANSLATION_ERROR;
}

bool ush_utils_is_printable(uint8_t ch)
{
        return ((ch >= 0x20) && (ch <= 0x7E));
}

static uint8_t hex_to_dec(char ch)
{
        if (ch >= '0' && ch <= '9') {
                return (uint8_t)(ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
                return (uint8_t)(ch - 'A') + 10;
        } else if (ch >= 'a' && ch <= 'f') {
                return (uint8_t)(ch - 'a') + 10;
        }
        return 0;
}

size_t ush_utils_decode_ascii(char *input, uint8_t *output, size_t max_size)
{
        char ch;
        int state = 0;
        uint8_t val;
        size_t ret_size = 0;

        while ((*input != '\0') && (ret_size < max_size)) {
                ch = *input;

                if (state == 0) {
                        switch (ch) {
                        case '\\':
                                state = 1;
                                break;
                        default:
                                *output++ = ch;
                                ret_size++;
                                break;
                        }
                } else if (state == 1) {
                        switch (ch) {
                        case 'x':
                                state = 2;
                                break;
                        default:
                                *output++ = ch;
                                ret_size++;
                                state = 0;
                                break;
                        }
                } else if (state == 2) {
                        val = hex_to_dec(ch) << 4;
                        state = 3;
                } else if (state == 3) {
                        val |= hex_to_dec(ch);
                        *output++ = val;
                        ret_size++;
                        state = 0;
                }
                
                input++;                
        }

        return ret_size;
}
