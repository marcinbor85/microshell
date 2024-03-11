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

#ifndef USH_COMMANDS_H
#define USH_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

/** Buildin global commands files array. */
extern const struct ush_file_descriptor g_ush_buildin_commands[];
/** Buildin global commands files array size. */
extern const size_t g_ush_buildin_commands_num;

/**
 * @brief Add global commands.
 * 
 * Function is used to add commands to shell global namespaces.
 * Commands are grouped by ush node object so every C module could have its own set of commands. 
 * 
 * @param self - pointer to master ush object
 * @param node - pointer to added ush node object supervisor of commands file array
 * @param file_list - pointer to added commands files array
 * @param file_list_size - added commands files array size
 * 
 * @return ush_status_t - USH_STATUS_OK when successful, otherwise error
 */
ush_status_t ush_commands_add(struct ush_object *self, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size);

/**
 * @brief Remove global commands.
 * 
 * Function is used to remove commands from shell global namespaces.
 * Commands are removed by group, registered to single ush node object.
 * 
 * @param self - pointer to master ush object
 * @param node - pointer to removed ush node object supervisor of commands file array
 * 
 * @return USH_STATUS_OK when successful, otherwise error
 */
ush_status_t ush_commands_remove(struct ush_object *self, struct ush_node_object *node);

#ifdef __cplusplus
}
#endif

#endif /* USH_COMMANDS_H */
