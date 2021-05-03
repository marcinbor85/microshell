#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ush.h"
#include "ush_utils.h"
#include "test_func.h"

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{
        test_func_deinit();
}

void test_print(void)
{
        ush_print(&g_ush, "test");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING("test\r\n[test /]$ ", g_write_buf);
}

void test_print_status(void)
{
        char resp_buf[TEST_FUNC_IO_BUFFER_SIZE];

        for (int i = 0; i < USH_STATUS__TOTAL_NUM; i++) {
                ush_status_t status = (ush_status_t)i;

                strcpy(resp_buf, ush_utils_get_status_string(status));
                strcat(resp_buf, "\r\n[test /]$ ");

                ush_print_status(&g_ush, i);
                test_func_read_all();
                TEST_ASSERT_EQUAL_STRING(resp_buf, g_write_buf);
        }
}

void test_print_no_newline(void)
{
        ush_print_no_newline(&g_ush, "test");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING("test[test /]$ ", g_write_buf);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_print);
        RUN_TEST(test_print_status);
        RUN_TEST(test_print_no_newline);

        return UNITY_END();
}
