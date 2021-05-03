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
        test_func_deinit();
}

void test_cmd_pwd_root(void)
{
        TEST_FUNC_ASK("pwd",
                "/\r\n"
                "[test /]$ "
        );
}

void test_cmd_pwd_misc(void)
{
        ush_node_set_current_dir(&g_ush, "/data");
        TEST_FUNC_ASK("pwd",
                "/data\r\n"
                "[test data]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/1/11/111");
        TEST_FUNC_ASK("pwd",
                "/dir/1/11/111\r\n"
                "[test 111]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/2/21");
        TEST_FUNC_ASK("pwd",
                "/dir/2/21\r\n"
                "[test 21]$ "
        );
}

void test_cmd_pwd_error(void)
{
        TEST_FUNC_ASK("pwd 1",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );
}

void test_cmd_pwd_help(void)
{
        TEST_FUNC_ASK("help pwd",
                "pwd: pwd\r\n"
                "\tPrint current working directory path.\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_pwd_root);
        RUN_TEST(test_cmd_pwd_error);
        RUN_TEST(test_cmd_pwd_help);
        RUN_TEST(test_cmd_pwd_misc);

        return UNITY_END();
}
