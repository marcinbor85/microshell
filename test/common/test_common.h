
#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <unity.h>

#include "ush.h"

#define TEST_COMMON_CMD_BUFFER_SIZE     256UL
#define TEST_COMMON_IO_BUFFER_SIZE      256UL

void test_common_init(void);
void test_common_reset(void);

extern struct ush_object g_ush;

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_H */
