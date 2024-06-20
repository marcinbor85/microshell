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

#ifndef USH_NODE_H
#define USH_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

/**
 * @brief Mount node path to shell.
 * 
 * Function used to mount new path to shell file system.
 * Mounting process automatically set initial file list to node.
 * In case of mounting to "/", current working path is also set to root.
 * Parent node must be existing.
 * 
 * @param self - pointer to master ush object
 * @param path - pointer to mounted point name
 * @param node - pointer to mounted ush node object supervisor of path
 * @param file_list - pointer to mounted path files array
 * @param file_list_size - mounted path files array size
 * 
 * @return ush_status_t - USH_STATUS_OK when successful, otherwise error
 */
ush_status_t ush_node_mount(struct ush_object *self, const char *path, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size);

/**
 * @brief Unmount node path from shell.
 * 
 * Function used to unmount path from shell file system.
 * Node must be existing.
 * 
 * @param self - pointer to master ush object
 * @param node - pointer to unmounted ush node object supervisor of path
 * 
 * @return ush_status_t - USH_STATUS_OK when successful, otherwise error
 */
ush_status_t ush_node_unmount(struct ush_object *self, const char *path);

/**
 * @brief Set current working path.
 * 
 * Path must be absolute.
 * 
 * @param self - pointer to master ush object
 * @param path - pointer to absolute path name to set
 * 
 * @return ush_status_t - USH_STATUS_OK when successful, otherwise error
 */
ush_status_t ush_node_set_current_dir(struct ush_object *self, const char *path);

/**
 * @brief Convert relative local path to absolute path.
 * 
 * Converting takes into account the current working path.
 * Input path could be relative as well as absolute.
 * It doesn't operate on the mounted paths, so paths may not be exist at all.
 * 
 * @param self - pointer to master ush object
 * @param in_path - pointer to input path name
 * @param abs_path - pointer to variable where absolute path will be placed
 */
void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *abs_path);

/**
 * @brief Deinitialize node recursively.
 * 
 * Function is used to deinitialize ush node objects safely.
 * The existence of functions is related to the dynamic building of the nodes tree.
 * 
 * @param self - pointer to master ush object
 * @param node - pointer to deinitialized ush node object supervisor of path
 */
void ush_node_deinit_recursive(struct ush_object *self, struct ush_node_object *node);

/**
 * @brief Get parent node by path.
 * 
 * Function is used to search parent node by path.
 * Searching is performed by modifying path to 1-level upper.
 * It checks whetever the path name have parent node or not.
 * It doesn't operate on the mounted paths, so paths may not be exist at all.
 * Path must be absolute.
 * 
 * @param self - pointer to master ush object
 * @param path - pointer to path name
 * 
 * @return pointer to ush node object when successful, otherwise NULL
 */
struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path);

/**
 * @brief Get node by path.
 * 
 * Function is used to search node by path.
 * It searches node in shell filesystem tree.
 * Path must be absolute.
 * 
 * @param self - pointer to master ush object
 * @param path - pointer to path name
 * 
 * @return pointer to ush node object when successful, otherwise NULL
 */
struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* USH_NODE_H */
