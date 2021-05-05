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

#ifndef USH_TYPES_H
#define USH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ush_preconfig.h"

/* Stringinize macros. */
#define USH_STRING(s)   USH_STRING_(s)
#define USH_STRING_(s)  #s

/** State values enumerators for return values. */
typedef enum {
        USH_STATUS_OK,                                  /**< Successfull operation */
        USH_STATUS_ERROR_NODE_NOT_FOUND,                /**< Node not found nor exists */
        USH_STATUS_ERROR_NODE_WITH_CHILDS,              /**< Cannot unmount node with childs */
        USH_STATUS_ERROR_NODE_WITHOUT_PARENT,           /**< Cannot mount node because there is no parent */
        USH_STATUS_ERROR_NODE_ALREADY_MOUNTED,          /**< Mount point already exists */
        USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR,          /**< Syntax error or unknown command */
        USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS,       /**< Wrong command arguments */
        USH_STATUS_ERROR_FILE_NOT_EXECUTABLE,           /**< Cannot execute file, file not executable */
        USH_STATUS_ERROR_FILE_NOT_WRITABLE,             /**< Cannot write file, file not writable */
        USH_STATUS_ERROR_FILE_NOT_READABLE,             /**< Cannot read file, file not readable */
        USH_STATUS_ERROR_FILE_NO_HELP,                  /**< Help for file is not available */
        USH_STATUS_ERROR_FILE_NOT_FOUND,                /**< File not found nor exists */
        USH_STATUS_ERROR_FILE_READ_ONLY,                /**< File read only */
        USH_STATUS__TOTAL_NUM,                          /**< Number of available statuses, must be last */
} ush_status_t;

/** State values enumerators for internal FSM handling. */
typedef enum {
        USH_STATE_RESET,                                /**< Initial reset state */
        USH_STATE_RESET_PROMPT,                         /**< Reset state with printing prompt */

        USH_STATE_PROMPT_PREFIX,                        /**< Printing prompt prefix state */
        USH_STATE_PROMPT_HOST,                          /**< Printing prompt host state */
        USH_STATE_PROMPT_SPACE,                         /**< Printing prompt space state */
        USH_STATE_PROMPT_PATH,                          /**< Printing prompt path state */
        USH_STATE_PROMPT_SUFFIX,                        /**< Printing prompt suffix state */

        USH_STATE_READ_PREPARE,                         /**< Prepare to read char state */
        USH_STATE_READ_CHAR,                            /**< Read char from input interface state */

        USH_STATE_PARSE_PREPARE,                        /**< Prepare to parse command state */
        USH_STATE_PARSE_SEARCH_ARG,                     /**< Search for command arguments state */
        USH_STATE_PARSE_QUOTE_ARG,                      /**< Search for quote end char state */
        USH_STATE_PARSE_STANDARD_ARG,                   /**< Read next argument char */

        USH_STATE_WRITE_CHAR,                           /**< Write char to output interface state */

        USH_STATE_PROCESS_START,                        /**< Shared start substate used by commands processing */
        USH_STATE_PROCESS_SERVICE,                      /**< Shared service substate used by commands processing */
        USH_STATE_PROCESS_FINISH,                       /**< Shared finish substate used by commands processing */

#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1
        USH_STATE_AUTOCOMP_PREPARE,                     /**< Autocompletation prepare state */
        USH_STATE_AUTOCOMP_CANDIDATES_START,            /**< Start searching candidates state */
        USH_STATE_AUTOCOMP_CANDIDATES_COUNT,            /**< Count candidates state */
        USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE,         /**< Optimise candidates state */
        USH_STATE_AUTOCOMP_CANDIDATES_PRINT,            /**< Print autocompletion candidates state */
        USH_STATE_AUTOCOMP_CANDIDATES_FINISH,           /**< Finish processing candidates state */
        USH_STATE_AUTOCOMP_PROMPT_PREPARE,              /**< Prepare to print prompt after autocompletation state */
        USH_STATE_AUTOCOMP_PROMPT,                      /**< Print prompt after autocompletation state */
        USH_STATE_AUTOCOMP_RECALL,                      /**< Recall and print previous command state */
        USH_STATE_AUTOCOMP_RECALL_SUFFIX,               /**< Recall and print only suffix state */
#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */

        USH_STATE__TOTAL_NUM,                           /**< Number of available states, must be last */
} ush_state_t;

/* Forward declarations. */
struct ush_object;
struct ush_file_descriptor;

/**
 * @brief File execute callback.
 * 
 * Function is called from ush service context when command or file is executed.
 * Before called, command arguments was parsed (text only).
 * 
 * @param self - pointer to master ush object
 * @param file - pointer to processed file descriptor
 * @param argc - number of parsed text arguments
 * @param argv - pointer to array of text arguments (zero-ended)
 */
typedef void (*ush_file_execute_callback)(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);

/**
 * @brief File process service callback.
 * 
 * Function is called from ush service context when command entered to processing state.
 * It can use shared processing-related fields in ush object.
 * 
 * @param self - pointer to master ush object
 * @param file - pointer to processed file descriptor
 */
typedef void (*ush_file_process_service)(struct ush_object *self, struct ush_file_descriptor const *file);

/**
 * @brief File data getter callback.
 * 
 * Function is called from ush service context when data read is necessary.
 * Data is passed by pointer modification, so it should be available after function return.
 * It operates on binary data, so data size is necessary.
 * 
 * @param self - pointer to master ush object
 * @param file - pointer to processed file descriptor
 * @param data - pointer to variable where data pointer will be placed
 * 
 * @return number of bytes successfully read
 */
typedef size_t (*ush_file_data_getter)(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data);

/**
 * @brief File data setter callback.
 * 
 * Function is called from ush service context when data write is necessary.
 * It operates on binary data, so data size is necessary.
 * 
 * @param self - pointer to master ush object
 * @param file - pointer to processed file descriptor
 * @param data - pointer to binary data to write
 * @param size - number of bytes to write
 */
typedef void (*ush_file_data_setter)(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size);

/**
 * @brief File descriptor structure.
 * 
 * This structure contains file-related constant information.
 * It should be placed in ROM, in case if runtime modification is not necessary.
 * */
struct ush_file_descriptor {
        char const *name;                               /**< Pointer to file name (required) */
        char const *description;                        /**< Pointer to file description (optional) */
        char const *help;                               /**< Pointer to file help manual (optional) */

        ush_file_execute_callback exec;                 /**< File execute callback (optional) */
        ush_file_data_getter get_data;                  /**< File data getter callback (optional) */
        ush_file_data_setter set_data;                  /**< File data setter callback (optional) */
        ush_file_process_service process;               /**< File process service callback (optional) */
};

/**
 * @brief Node object structure.
 * 
 * This structure contains node-related information.
 * It is a supervisor of files and child nodes.
 * Could be modified runtime, so should be placed in RAM.
 * Implements dynamic list of dynamic list structure.
 */
struct ush_node_object {
        struct ush_file_descriptor const *file_list;    /**< Pointer to file descriptor array */
        size_t file_list_size;                          /**< Size of file descriptor array */
        char const *path;                               /**< Node path, set after node mount */

        struct ush_node_object *parent;                 /**< Pointer to parent node if not root */
        struct ush_node_object *childs;                 /**< Pointer to childs node (1-level down) */
        struct ush_node_object *next;                   /**< Pointer to next node (on the same parent) */
};

/**
 * @brief Read char interface callback.
 * 
 * Function is called from ush service context when library wants to read char from input shell interface.
 * When used in asynchronous environment, it should not block and should return status.
 * When used in synchronous environment, it could block, but ush service also will be blocked.
 * It should read only 1 byte at once.
 * 
 * @param self - pointer to master ush object
 * @param ch - pointer to char variable where read byte should be placed
 * 
 * @return should return 1 on read successful, otherwise 0 when no data avaialble
 */
typedef int (*ush_io_interface_read_char)(struct ush_object *self, char *ch);

/**
 * @brief Write char interface callback.
 * 
 * Function is called from ush service context when library wants to write char to output shell interface.
 * When used in asynchronous environment, it should not block and should return status.
 * When used in synchronous environment, it could block, but ush service also will be blocked.
 * It should write only 1 byte at once.
 * 
 * @param self - pointer to master ush object
 * @param ch - char to write
 * 
 * @return should return 1 on write successful, otherwise 0 when cannot write data
 */
typedef int (*ush_io_interface_write_char)(struct ush_object *self, char ch);

/**
 * @brief IO interface structure.
 * 
 * This structure contains interface-related information.
 * It should be placed in ROM, in case if runtime modification is not necessary.
 */
struct ush_io_interface {
        ush_io_interface_read_char read;                /**< Read char interface callback */
        ush_io_interface_write_char write;              /**< Write char interface callback */
};

/**
 * @brief Shell main descriptor structure.
 * 
 * This structure contains shell-related information.
 * It should be placed in ROM, in case if runtime modification is not necessary.
 */
struct ush_descriptor {
        struct ush_io_interface const *io;              /**< Pointer to IO interface structure */
        char *input_buffer;                             /**< Pointer to input working buffer (used to prepare and parsing data) */
        size_t input_buffer_size;                       /**< Input working buffer size */
        char *output_buffer;                            /**< Pointer to output working buffer (used to prepare and printing data) */
        size_t output_buffer_size;                      /**< Output working buffer size */
        size_t path_max_length;                         /**< Path maximum length (stack allocated) */
        char *hostname;                                 /**< Pointer to host name (used in prompt) */

        ush_file_execute_callback exec;                 /**< General command execute callback (optional) */
};

/**
 * @brief Shell main object structure.
 * 
 * This structure contains shell-related current data.
 * Works like a shell instance, every function used it context.
 * It must be placed in RAM.
 */
struct ush_object {
        struct ush_descriptor const *desc;              /**< Pointer to shell main descriptor */

        ush_state_t state;                              /**< Current FSM internal state */
        ush_state_t write_next_state;                   /**< FSM internal state set after write */
        ush_state_t prompt_next_state;                  /**< FSM internal state set after prompt */

        char *write_buf;                                /**< Pointer to data to write to output interface */
        size_t write_size;                              /**< Number of chars to write to output interface */
        size_t write_pos;                               /**< Current data position during write to output interface */

        int ansi_escape_state;                          /**< Escape state during parsing arguments */
        size_t in_pos;                                  /**< Current input working buffer position */
        size_t out_pos;                                 /**< Current output working buffer position */
        size_t args_count;                              /**< Number of arguments (holding between misc states) */
        bool escape_flag;                               /**< Escape quote flag */

        struct ush_node_object *commands;               /**< Pointer to commands in global namespace */
        struct ush_node_object *root;                   /**< Pointer to root node */
        struct ush_node_object *current_node;           /**< Pointer to current working path node */

        struct ush_node_object buildin_commands;        /**< Buildin node object as a buildin commands supervisor */

        struct ush_node_object *process_node;           /**< Pointer to current processed node */
        struct ush_node_object *process_child_node;     /**< Pointer to current processed child node */
        struct ush_file_descriptor const *process_file; /**< Pointer to current processed file */

        size_t process_index;                           /**< Shared processed index */
        size_t process_index_item;                      /**< Shared processed item index */
        uint8_t *process_data;                          /**< Shared processed data pointer */
        size_t process_data_size;                       /**< Shared processed data size */
        int process_stage;                              /**< Shared processed stage number */

#if USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE == 1
        ush_state_t autocomp_prev_state;                /**< Previous autocompletation FSM state */

        char *autocomp_input;                           /**< Pointer to input autocompletation data */
        char *autocomp_candidate_name;                  /**< Pointer to autocompletation candidate name */
        char *autocomp_name;                            /**< Pointer to autocompletation intermediate candidate name */

        size_t autocomp_count;                          /**< Current number of candidates */
        size_t autocomp_prev_count;                     /**< Previous number of candidates */
        size_t autocomp_suffix_len;                     /**< Autocompletation suffix length */
#endif /* USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE */
};

#ifdef __cplusplus
}
#endif

#endif /* USH_TYPES_H */
