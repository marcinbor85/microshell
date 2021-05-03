#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ush.h"
#include "test_func.h"

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{

}

void test_cmd_echo_shell(void)
{
        test_func_ask("echo",
                "\r\n"
                "[test /]$ "
        );

        test_func_ask("echo abc",
                "abc\r\n"
                "[test /]$ "
        );

        test_func_ask("echo \"a b c\"",
                "a b c\r\n"
                "[test /]$ "
        );
}

void test_cmd_echo_error(void)
{
        test_func_ask("echo 1 2",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        test_func_ask("echo 1 2 3",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        test_func_ask("echo 1 > 2 3",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        test_func_ask("echo 1 > qwerty",
                "error: file not found\r\n"
                "[test /]$ "
        );

        test_func_ask("echo \"a b c\" > qwerty",
                "error: file not found\r\n"
                "[test /]$ "
        );

        test_func_ask("echo abc > /test",
                "error: file not writable\r\n"
                "[test /]$ "
        );

        test_func_ask("echo abc > test",
                "error: file not writable\r\n"
                "[test /]$ "
        );
}

void test_cmd_echo_help(void)
{
        test_func_ask("help echo",
                "echo: echo [string] > [file]\r\n"
                "\tPrint string to file.\r\n"
                "      echo [string] \r\n"
                "\tPrint string to console.\r\n"
                "[test /]$ "
        );
}

void test_cmd_echo_redirect(void)
{
        test_func_ask("echo abc > /data/null",
                "[test /]$ "
        );

        TEST_ASSERT_EQUAL_UINT8_ARRAY("\0\0\0\0", g_file_buffer_buf, 4);
        test_func_ask("echo abc > /data/buffer",
                "[test /]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("abc\0", g_file_buffer_buf, 4);

        ush_node_set_current_dir(&g_ush, "/data");

        TEST_ASSERT_EQUAL_UINT8_ARRAY("abc\0", g_file_buffer_buf, 4);
        test_func_ask("echo \"xy\" > buffer",
                "[test data]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("xyc\0", g_file_buffer_buf, 4);

        TEST_ASSERT_EQUAL_UINT8_ARRAY("xyc\0\0\0\0", g_file_buffer_buf, 7);
        test_func_ask("echo \"qwe rty\" > ./buffer",
                "[test data]$ "
        );
        TEST_ASSERT_EQUAL_UINT8_ARRAY("qwe rty\0", g_file_buffer_buf, 7);
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
