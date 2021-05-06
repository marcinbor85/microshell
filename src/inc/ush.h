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

#ifndef USH_H
#define USH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"
#include "ush_const.h"
#include "ush_preconfig.h"
#include "ush_shell.h"

#include "ush_file.h"
#include "ush_node.h"
#include "ush_utils.h"
#include "ush_commands.h"

/**
 * @brief Shell initialization.
 * 
 * Main function to initialize shell instance.
 * Should be called once before mounting user-specified nodes.
 * Could be deinitialized later with ush_deinit function.
 * It doesn't check descriptor callback or any other arguments runtime.
 * Only assert could be invoked in case of errors.
 * 
 * @param self - pointer to master ush object
 * @param desc - pointer to master ush descriptor
 */
void ush_init(struct ush_object *self, const struct ush_descriptor *desc);

/**
 * @brief Shell deinitialization.
 * 
 * Function to deinitialize shell instance.
 * It clears all variables and recursive deinitialize mounted nodes.
 * 
 * @param self - pointer to master ush object
 */
void ush_deinit(struct ush_object *self);

/**
 * @brief Shell service function.
 * 
 * Function to handle all internal states and processing logic.
 * All callbacks and interface operations are called from it context.
 * In case of asynchronous non-blocking environment it should be called periodically.
 * In case of synchronous blocking environment, it should be called in loop until return false.
 * 
 * @param self - pointer to master ush object
 * 
 * @return true - in case of "busy, processing ongoing", false - "nothing to do, read input interface"
 */
bool ush_service(struct ush_object *self);

/**
 * @brief Shell reset function.
 * 
 * Function to software reset shell.
 * After reset, shell starts with welcome message.
 * Current working path will be set to root.
 * 
 * @param self - pointer to master ush object
 */
void ush_reset(struct ush_object *self);

/**
 * @brief Print status message.
 * 
 * Function to print status message to output shell interface.
 * 
 * @param self - pointer to master ush object
 * @param status - status to print
 */
void ush_print_status(struct ush_object *self, ush_status_t status);

/**
 * @brief Print custom message without newline.
 * 
 * Function to print custom message without newline to output shell interface.
 * 
 * @param self - pointer to master ush object
 * @param buf - pointer to text message to print
 */
void ush_print_no_newline(struct ush_object *self, char *buf);

/**
 * @brief Print custom message.
 * 
 * Function to print custom message to output shell interface.
 * 
 * @param self - pointer to master ush object
 * @param buf - pointer to text message to print
 */
void ush_print(struct ush_object *self, char *buf);

/**
 * @brief Flush output buffer.
 * 
 * Function to print output buffer content to shell output interface.
 * 
 * @param self - pointer to master ush object
 */
void ush_flush(struct ush_object *self);

#ifdef __cplusplus
}
#endif

#endif /* USH_H */
