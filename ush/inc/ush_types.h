
#ifndef USH_TYPES_H
#define USH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ush_config.h"

typedef enum {
        USH_STATE_RESET,

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

        USH_STATE__TOTAL_NUM,
} ush_state_t;

typedef enum {
        USH_MESSAGE_ERROR_UNKNOWN_COMMAND,
        USH_MESSAGE_ERROR_WRONG_ARGUMENTS,
        USH_MESSAGE_ERROR_DIRECTORY_NOT_FOUND,
        USH_MESSAGE_ERROR_COMMAND_NOT_FOUND,
        USH_MESSAGE_ERROR_NO_HELP_AVAILABLE,
        USH_MESSAGE__TOTAL_NUM,
} ush_message_t;

struct ush_object;
struct ush_cmd_descriptor;

typedef char* (*ush_cmd_callback)(struct ush_object *self, struct ush_cmd_descriptor const *cmd, int argc, char *argv[]);

struct ush_cmd_descriptor {
        char const *name;
        char const *description;
        char const *help;

        ush_cmd_callback cmd_callback;
};

struct ush_path_object {
        struct ush_cmd_descriptor const *cmd_list;
        size_t cmd_list_size;
        char const *mount_point;
        char const *name;
        
        struct ush_path_object *next;
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
        char *output_buffer;
        size_t output_buffer_size;
        char *hostname;

        ush_cmd_callback cmd_callback;
};

struct ush_object {
        struct ush_descriptor const *desc;

        ush_state_t state;
        ush_state_t write_next_state;

        char *write_buf;
        size_t write_size;
        size_t write_pos;

        size_t in_pos;
        size_t out_pos;
        size_t args_count;
        bool escape_flag;

        struct ush_path_object *path_first;
        struct ush_path_object *current_path;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_TYPES_H */
