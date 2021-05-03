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

void test_cmd_pwd_root(void)
{
        test_func_ask("pwd",
                "/\r\n"
                "[test /]$ "
        );
}

void test_cmd_pwd_misc(void)
{
        ush_node_set_current_dir(&g_ush, "/data");
        test_func_ask("pwd",
                "/data\r\n"
                "[test data]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/1/11/111");
        test_func_ask("pwd",
                "/dir/1/11/111\r\n"
                "[test 111]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/2/21");
        test_func_ask("pwd",
                "/dir/2/21\r\n"
                "[test 21]$ "
        );
}

void test_cmd_pwd_error(void)
{
        test_func_ask("pwd 1",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );
}

void test_cmd_pwd_help(void)
{
        test_func_ask("help pwd",
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
