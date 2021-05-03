#ifndef USH_FILE_H
#define USH_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_types.h"

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* USH_FILE_H */
