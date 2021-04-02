
#ifndef USH_TYPES_H
#define USH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct ush_object;

typedef int (*ush_io_iface_read_char)(struct ush_object *self, char *ch);
typedef int (*ush_io_iface_write_char)(struct ush_object *self, char ch);

struct ush_io_interface {
        ush_io_iface_read_char read;
        ush_io_iface_write_char write;
};

struct ush_descriptor {
        struct ush_io_interface const *iface;
        char *cmd_buffer;
        size_t cmd_buffer_size;
};

struct ush_object {
        struct ush_descriptor const *desc;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_TYPES_H */
