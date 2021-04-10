#ifndef USH_UTILS_H
#define USH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

size_t ush_utils_get_path_levels_count(const char *path);
void ush_utils_get_collapse_path(char *in_path, char *out_path);
void ush_utils_path_upper(char *path);
void ush_utils_path_last(const char *in_path, char* *out_path);
void ush_utils_get_path_level(size_t level, const char *in_path, char *out_path);
void ush_utils_join_path(const char *in_path, const char *name, char *out_path);

#ifdef __cplusplus
}
#endif

#endif /* USH_UTILS_H */
