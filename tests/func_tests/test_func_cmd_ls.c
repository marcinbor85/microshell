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

void test_cmd_ls_current(void)
{
        TEST_FUNC_ASK("ls",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test /]$ "
        );

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/data"));

        TEST_FUNC_ASK("ls",
                "d---- .\r\n"
                "d---- ..\r\n"
                "----- text\r\n"
                "----- text_file1\r\n"
                "----- text_file2\r\n"
                "-rw-- null\r\n"
                "-rw-- buffer\r\n"
                "-r--- binary\r\n"
                "[test data]$ "
        );

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/dir"));

        TEST_FUNC_ASK("ls",
                "d---- .\r\n"
                "d---- ..\r\n"
                "d---- 2/\r\n"
                "d---- 1/\r\n"
                "[test dir]$ "
        );
}

void test_cmd_ls_abs(void)
{
        TEST_FUNC_ASK("ls /dir/1",
                "d---- .\r\n"
                "d---- ..\r\n"
                "d---- 13/\r\n"
                "d---- 12/\r\n"
                "d---- 11/\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls /dir/../",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test /]$ "
        );

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/dir/1/11/111"));

        TEST_FUNC_ASK("ls /dir/1",
                "d---- .\r\n"
                "d---- ..\r\n"
                "d---- 13/\r\n"
                "d---- 12/\r\n"
                "d---- 11/\r\n"
                "[test 111]$ "
        );

        TEST_FUNC_ASK("ls /dir/../",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test 111]$ "
        );
}

void test_cmd_ls_rel(void)
{
        TEST_FUNC_ASK("ls ..",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls ./../../..",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls ./../",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls dir/..",
                "d---- .\r\n"
                "d---- dir/\r\n"
                "d---- data/\r\n"
                "---xh test             - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls dir/../dir/1",
                "d---- .\r\n"
                "d---- ..\r\n"
                "d---- 13/\r\n"
                "d---- 12/\r\n"
                "d---- 11/\r\n"
                "[test /]$ "
        );

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/dir/1/11/111"));

        TEST_FUNC_ASK("ls ..",
                "d---- .\r\n"
                "d---- ..\r\n"
                "d---- 111/\r\n"
                "[test 111]$ "
        );

        TEST_FUNC_ASK("ls ../../../../data",
                "d---- .\r\n"
                "d---- ..\r\n"
                "----- text\r\n"
                "----- text_file1\r\n"
                "----- text_file2\r\n"
                "-rw-- null\r\n"
                "-rw-- buffer\r\n"
                "-r--- binary\r\n"
                "[test 111]$ "
        );

        TEST_FUNC_ASK("ls",
                "d---- .\r\n"
                "d---- ..\r\n"
                "----- joke\r\n"
                "[test 111]$ "
        );
}

void test_cmd_ls_error(void)
{
        TEST_FUNC_ASK("ls 1 2",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls /test",
                "error: directory not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls di",
                "error: directory not found\r\n"
                "[test /]$ "
        );
}

void test_cmd_ls_help(void)
{
        TEST_FUNC_ASK("help ls",
                "ls: ls [path]\r\n"
                "\tList directory content.\r\n"
                "    ls\r\n"
                "\tList current directory content.\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_ls_current);
        RUN_TEST(test_cmd_ls_abs);
        RUN_TEST(test_cmd_ls_rel);
        RUN_TEST(test_cmd_ls_error);
        RUN_TEST(test_cmd_ls_help);

        return UNITY_END();
}
