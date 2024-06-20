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

static size_t fill_string(char *string, int len)
{
        memset(string, 'T', len);
        string[len - 1] = '\0';
        return strlen(string);
}

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

void test_printf(void)
{
        /* Test with formatting options and concatenation when test_string has been
           partially read by first test_func_read() */
        char test_string[] = "line%s%d\r\n";
        ush_printf(&g_ush, test_string, "string", 0);
        test_func_read(true, strlen(test_string) / 2);
        ush_printf(&g_ush, test_string, "string", 1);
        test_func_read(false, INT_MAX);
        TEST_ASSERT_EQUAL_STRING("linestring0\r\n"
                                 "linestring1\r\n"
                                 "[test /]$ ",
                                 g_write_buf);
}

void test_printf_long(void)
{
        /* Test with longest possible string */
        char test_string[TEST_FUNC_WORK_BUFFER_SIZE];
        size_t test_string_len = fill_string(test_string, sizeof(test_string));
        ush_printf(&g_ush, "%s", test_string);
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING_LEN(test_string, g_write_buf, test_string_len);
        TEST_ASSERT_EQUAL_STRING("[test /]$ ", &g_write_buf[test_string_len]);
}

void test_printf_format_error(void)
{
        /* Test simple string and and then concatenate invalid formatting options */
        ush_printf(&g_ush, "string0");
        ush_printf(&g_ush, "%lc", 0xffffffff);
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING("string0...format error\r\n"
                                 "[test /]$ ",
                                 g_write_buf);
}

void test_printf_overflow_error(void)
{
        /* Test with two simple strings and then concatenate overflow_string */
        const char first_string[] = "string0";
        char test_string[TEST_FUNC_WORK_BUFFER_SIZE - strlen(first_string)];
        (void)fill_string(test_string, sizeof(test_string));
        const char overflow_string[] = "O";
        ush_printf(&g_ush, "%s", first_string);
        ush_printf(&g_ush, "%s", test_string);
        ush_printf(&g_ush, "%s", overflow_string);
        test_func_read_all();
        TEST_ASSERT_LESS_THAN_size_t(TEST_FUNC_WORK_BUFFER_SIZE, strlen(g_write_buf));
        TEST_ASSERT_EQUAL_STRING_LEN(first_string, g_write_buf, strlen(first_string));
        char *overflow_msg_start = strchr(g_write_buf, '.');
        TEST_ASSERT_EQUAL_STRING("...overflow error\r\n", overflow_msg_start);
        size_t remain_test_string_len = strlen(test_string) - strlen(overflow_msg_start);
        TEST_ASSERT_EQUAL_STRING_LEN(test_string, g_write_buf + strlen(first_string), remain_test_string_len);
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

void test_flush(void)
{
        strcpy(g_ush.desc->output_buffer, "test123\r\n");
        ush_flush(&g_ush);
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING("test123\r\n[test /]$ ", g_write_buf);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_print);
        RUN_TEST(test_printf);
        RUN_TEST(test_printf_long);
        RUN_TEST(test_printf_format_error);
        RUN_TEST(test_printf_overflow_error);
        RUN_TEST(test_print_status);
        RUN_TEST(test_print_no_newline);
        RUN_TEST(test_flush);

        return UNITY_END();
}
