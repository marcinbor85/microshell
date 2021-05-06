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

void test_input_empty(void)
{
        TEST_FUNC_ASK("",
                "[test /]$ "
        );
}

void test_input_error(void)
{
        TEST_FUNC_ASK("abcd",
                "error: syntax error\r\n"
                "[test /]$ "
        );
}

void test_input_backspace(void)
{
        test_func_write("test\x08\x08\x08\x08\n");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "test\x08 \x08\x08 \x08\x08 \x08\x08 \x08\n"
                "[test /]$ ",
                g_write_buf
        );

        test_func_write("pwx\x08""d\n");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "pwx\x08 \x08""d\n"
                "/\r\n"
                "[test /]$ ",
                g_write_buf
        );

        test_func_write("t\x08\x08\x08\x08""pw\x08""wd\n");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "t\x08 \x08""pw\x08 \x08""wd\n"
                "/\r\n"
                "[test /]$ ",
                g_write_buf
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_input_empty);
        RUN_TEST(test_input_error);
        RUN_TEST(test_input_backspace);

        return UNITY_END();
}
