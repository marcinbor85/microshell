
#ifndef USH_TYPES_H
#define USH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ush_config.h"

#define USH_PROCESS_WRITE_MAX_LEN       3
#define USH_ECHO_BUFFER_SIZE            4

typedef enum {
        USH_STATUS_OK,
        USH_STATUS_ERROR_NODE_NOT_EXISTS,
        USH_STATUS_ERROR_NODE_WITH_CHILDS,
        USH_STATUS_ERROR_NODE_WITHOUT_PARENT,
        USH_STATUS_ERROR_NODE_ALREADY_MOUNTED,
        USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR,
        USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS,
        USH_STATUS_ERROR_COMMAND_NOT_EXECUTABLE,
        USH_STATUS_ERROR_COMMAND_WITHOUT_HELP,
        USH_STATUS__TOTAL_NUM,
} ush_status_t;

typedef enum {
        USH_STATE_RESET,
        USH_STATE_RESET_PROMPT,

        USH_STATE_PROMPT_PREFIX,
        USH_STATE_PROMPT_HOST,
        USH_STATE_PROMPT_SPACE,
        USH_STATE_PROMPT_PATH,
        USH_STATE_PROMPT_SUFFIX,

        USH_STATE_READ_PREPARE,
        USH_STATE_READ_CHAR,

        USH_STATE_PARSE_PREPARE,
        USH_STATE_PARSE_SEARCH_ARG,
        USH_STATE_PARSE_QUOTE_ARG,
        USH_STATE_PARSE_STANDARD_ARG,
        USH_STATE_PARSE_SEARCH_STOP,

        USH_STATE_WRITE_CHAR,

        USH_STATE_PROCESS_START,
        USH_STATE_PROCESS_SERVICE,
        USH_STATE_PROCESS_FINISH,

        USH_STATE_AUTOCOMP_PREPARE,
        USH_STATE_AUTOCOMP_CANDIDATES_COUNT,
        USH_STATE_AUTOCOMP_CANDIDATES_SEARCH,
        USH_STATE_AUTOCOMP_CANDIDATES_FINISH,
        USH_STATE_AUTOCOMP_PROMPT_PREPARE,
        USH_STATE_AUTOCOMP_PROMPT,
        USH_STATE_AUTOCOMP_RECALL,

        USH_STATE__TOTAL_NUM,
} ush_state_t;

struct ush_object;
struct ush_file_descriptor;

typedef void (*ush_file_execute_callback)(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
typedef void (*ush_file_process_service)(struct ush_object *self);

struct ush_file_descriptor {
        char const *name;
        char const *description;
        char const *help;

        ush_file_execute_callback exec;
        ush_file_process_service process;
};

struct ush_node_object {
        struct ush_file_descriptor const *file_list;
        size_t file_list_size;
        char const *path;

        struct ush_node_object *parent;
        struct ush_node_object *childs;
        struct ush_node_object *next;
};

typedef int (*ush_io_interface_read_char)(struct ush_object *self, char *ch);
typedef int (*ush_io_interface_write_char)(struct ush_object *self, char ch);

struct ush_io_interface {
        ush_io_interface_read_char read;
        ush_io_interface_write_char write;
};

struct ush_descriptor {
        struct ush_io_interface const *io;
        char *input_buffer;
        size_t input_buffer_size;
        char *hostname;

        ush_file_execute_callback exec;
};

struct ush_object {
        struct ush_descriptor const *desc;

        ush_state_t state;
        ush_state_t write_next_state;
        ush_state_t prompt_next_state;

        char *write_buf;
        size_t write_size;
        size_t write_pos;
        char echo_buf[USH_ECHO_BUFFER_SIZE];

        int ansi_escape_state;
        size_t in_pos;
        size_t out_pos;
        size_t args_count;
        bool escape_flag;

        struct ush_node_object *commands;
        struct ush_node_object *root;
        struct ush_node_object *current_node;

        struct ush_node_object buildin_commands;

        struct ush_node_object *process_node;
        struct ush_node_object *process_child_node;
        struct ush_file_descriptor const *process_file;

        size_t process_index;
        size_t process_index_item;

        char *autocomp_input;
        char *autocomp_candidate_name;
        bool autocomp_print_flag;
        size_t autocomp_count;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_TYPES_H */
