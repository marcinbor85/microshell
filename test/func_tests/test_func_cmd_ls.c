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

void test_cmd_ls_current(void)
{
        TEST_FUNC_ASK("ls",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );

        ush_node_set_current_dir(&g_ush, "/data");

        TEST_FUNC_ASK("ls",
                ".\r\n"
                "..\r\n"
                "text            - text file\r\n"
                "text_file1      - text file 1\r\n"
                "text_file2      - text file 2\r\n"
                "null            - null file\r\n"
                "buffer          - buffer file\r\n"
                "binary          - binary file\r\n"
                "[test data]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir");

        TEST_FUNC_ASK("ls",
                ".\r\n"
                "..\r\n"
                "2/\r\n"
                "1/\r\n"
                "[test dir]$ "
        );
}

void test_cmd_ls_abs(void)
{
        TEST_FUNC_ASK("ls /dir/1",
                ".\r\n"
                "..\r\n"
                "13/\r\n"
                "12/\r\n"
                "11/\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls /dir/../",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/1/1/1");

        TEST_FUNC_ASK("ls /dir/1",
                ".\r\n"
                "..\r\n"
                "13/\r\n"
                "12/\r\n"
                "11/\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls /dir/../",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );
}

void test_cmd_ls_rel(void)
{
        TEST_FUNC_ASK("ls ..",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls ./../../..",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls ./../",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls dir/..",
                ".\r\n"
                "dir/\r\n"
                "data/\r\n"
                "test            - test file\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls dir/../dir/1",
                ".\r\n"
                "..\r\n"
                "13/\r\n"
                "12/\r\n"
                "11/\r\n"
                "[test /]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/1/11/111");

        TEST_FUNC_ASK("ls ..",
                ".\r\n"
                "..\r\n"
                "111/\r\n"
                "[test 111]$ "
        );

        TEST_FUNC_ASK("ls ../../../../data",
                ".\r\n"
                "..\r\n"
                "text            - text file\r\n"
                "text_file1      - text file 1\r\n"
                "text_file2      - text file 2\r\n"
                "null            - null file\r\n"
                "buffer          - buffer file\r\n"
                "binary          - binary file\r\n"
                "[test 111]$ "
        );
}

void test_cmd_ls_error(void)
{
        TEST_FUNC_ASK("ls 1 2",
                "error: wrong arguments\r\n"
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

void test_cmd_ls_not_exist(void)
{
        TEST_FUNC_ASK("ls /test",
                "error: directory not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("ls di",
                "error: directory not found\r\n"
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
        RUN_TEST(test_cmd_ls_not_exist);

        return UNITY_END();
}
