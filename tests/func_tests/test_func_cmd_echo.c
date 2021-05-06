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

void test_cmd_echo_shell(void)
{
        TEST_FUNC_ASK("echo",
                "\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo abc",
                "abc\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo \"a b c\"",
                "a b c\r\n"
                "[test /]$ "
        );
}

void test_cmd_echo_error(void)
{
        TEST_FUNC_ASK("echo 1 2",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo 1 2 3",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo 1 > 2 3",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo 1 > qwerty",
                "error: file not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo \"a b c\" > qwerty",
                "error: file not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo abc > /test",
                "error: file not writable\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("echo abc > test",
                "error: file not writable\r\n"
                "[test /]$ "
        );
}

void test_cmd_echo_help(void)
{
        TEST_FUNC_ASK("help echo",
                "usage: echo <string> > <file>\r\n"
                "       echo [string]\r\n"
                "       * can be ascii-hex encoded string with \\xNN format.\r\n"
                "[test /]$ "
        );
}

void test_cmd_echo_redirect(void)
{
        TEST_FUNC_ASK("echo abc > /data/null",
                "[test /]$ "
        );

        TEST_ASSERT_EQUAL_UINT8_ARRAY("\0\0\0\0", g_file_buffer_buf, 4);
        TEST_FUNC_ASK("echo abc > /data/buffer",
                "[test /]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("abc\0", g_file_buffer_buf, 4);

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/data"));

        TEST_ASSERT_EQUAL_UINT8_ARRAY("abc\0", g_file_buffer_buf, 4);
        TEST_FUNC_ASK("echo \"xy\" > buffer",
                "[test data]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("xyc\0", g_file_buffer_buf, 4);

        TEST_ASSERT_EQUAL_UINT8_ARRAY("xyc\0\0\0\0", g_file_buffer_buf, 7);
        TEST_FUNC_ASK("echo \"qwe rty\" > ./buffer",
                "[test data]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("qwe rty\0", g_file_buffer_buf, 7);

        TEST_ASSERT_EQUAL_UINT8_ARRAY("qwe rty\0", g_file_buffer_buf, 7);
        TEST_FUNC_ASK("echo \"\\x00a cd\\x00\\xAA\" > ./buffer",
                "[test data]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("\0a cd\0\xAA", g_file_buffer_buf, 7);

        TEST_FUNC_ASK("echo "
                "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                "q > buffer",
                "[test data]$ "
        );
        TEST_ASSERT_EACH_EQUAL_UINT8('a', g_file_buffer_buf, 128);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_echo_shell);
        RUN_TEST(test_cmd_echo_error);
        RUN_TEST(test_cmd_echo_help);
        RUN_TEST(test_cmd_echo_redirect);

        return UNITY_END();
}
