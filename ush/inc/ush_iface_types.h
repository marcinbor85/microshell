#ifndef USH_IFACE_TYPES_H
#define USH_IFACE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct ush_object;

typedef int (*ush_iface_read_char)(struct ush_object *self, char *ch);
typedef int (*ush_iface_write_char)(struct ush_object *self, char ch);

struct ush_iface {
        ush_iface_read_char read;
        ush_iface_write_char write;
};

#ifdef __cplusplus
}
#endif

#endif /* USH_IFACE_TYPES_H */
