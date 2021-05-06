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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <microshell.h>

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
