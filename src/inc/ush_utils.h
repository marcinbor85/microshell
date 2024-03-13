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

#ifndef USH_UTILS_H
#define USH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

/**
 * @brief Count path levels.
 * 
 * Function used to count path levels.
 * Path must be absolute.
 * 
 * @param path - pointer to path string
 * 
 * @return number of levels
 */
size_t ush_utils_get_path_levels_count(const char *path);

/**
 * @brief Collapse path.
 * 
 * Function used to collapse relative path to absolute.
 * It handles ".." and "." path parts.
 * Doesn't use shell mounted file system, so paths could not exist.
 * Path could be relative or absolute.
 * 
 * @param in_path - pointer to input path string (will be modified, so must be placed in RAM)
 * @param out_path - pointer to output path string
 */
void ush_utils_get_collapse_path(char *in_path, char *out_path);

/**
 * @brief Collapse path 1-level higher.
 * 
 * Function used to collapse path 1-level higher.
 * Path could be relative or absolute.
 * 
 * @param path - pointer to path string (will be modified, so must be placed in RAM)
 */
void ush_utils_path_upper(char *path);

/**
 * @brief Get last part of path
 * 
 * Function used to get last part of path.
 * It is the same as lowest level of path.
 * Path could be relative or absolute.
 * Never returns NULL, at least input string.
 * 
 * @param in_path - pointer to input path string
 * 
 * @return pointer to last part of path
 */
char* ush_utils_path_last(const char *in_path);

/**
 * @brief Get path with specified level.
 * 
 * Function used to get path with specified level.
 * It based on the input path.
 * It doesn't modify source path, because it is common to use this function in loops.
 * Output path will be always shorter than input, so user have to ensure that output buffer will be long enough.
 * Path must be be absolute.
 * 
 * @param level - level of path
 * @param in_path - pointer to input path string
 * @param out_path - pointer to place where new path will be placed
 */
void ush_utils_get_path_level(size_t level, const char *in_path, char *out_path);

/**
 * @brief Join path and file name.
 * 
 * Function used to join path and file name.
 * User have to ensure that output buffer will be long enough to join them.
 * Path could be relative or absolute.
 * 
 * @param in_path - pointer to input path string
 * @param name - pointer to file name to join
 * @param out_path - pointer to place where new joined path will be placed
 */
void ush_utils_join_path(const char *in_path, const char *name, char *out_path);

/**
 * @brief Get last argument.
 * 
 * Function used to get last argument from string.
 * Arguments are separated by spaces.
 * Never returns NULL, at least input string.
 * 
 * @param input - pointer to input string
 * 
 * @return pointer to last argument
 */
char* ush_utils_get_last_arg(const char *input);

/**
 * @brief Check if string starts with prefix.
 * 
 * Function used to check if input string starts with prefix string.
 * 
 * @param input - pointer to input string
 * @param prefix - pointer to prefix string
 * 
 * @return true or false
 */
bool ush_utils_startswith(const char *input, char *prefix);

/**
 * @brief Check if char is printable.
 * 
 * Function used to check if char is ASCII printable.
 * Printable chars are in range of 0x20 to 0x7E.
 * 
 * @param ch - input char
 * 
 * @return true or false
 */
bool ush_utils_is_printable(uint8_t ch);

/**
 * @brief Decode ASCII encoded string to binary.
 * 
 * Function used to convert ASCII text string to binary.
 * It used "\" escape character followed by "x" to encode byte hex numbers.
 * It accept lower and upper hex chars.
 * When hex char is invalid, it is overridden by 0.
 * 
 * @param input - pointer to input string (null-terminated)
 * @param output - pointer to place where will be placed binary data
 * @param max_size - output buffer maximum size
 * 
 * @return number of bytes successfully written to output buffer
 */
size_t ush_utils_decode_ascii(char *input, uint8_t *output, size_t max_size);

/**
 * @brief Get text representation of status.
 * 
 * Function used to convert status enum to string.
 * 
 * @param status - status to convert
 * 
 * @return pointer to string with text representation of status
 */
const char* ush_utils_get_status_string(ush_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* USH_UTILS_H */
