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

void test_pwd_root(void)
{
        test_func_ask("pwd",
                "/\r\n"
                "[test /]$ "
        );
}

void test_pwd_misc(void)
{
        test_func_ask("cd data",
                "[test data]$ "
        );
        test_func_ask("pwd",
                "/data\r\n"
                "[test data]$ "
        );
        test_func_ask("cd /dir/1/11/111",
                "[test 111]$ "
        );
        test_func_ask("pwd",
                "/dir/1/11/111\r\n"
                "[test 111]$ "
        );
}

void test_pwd_error(void)
{
        test_func_ask("pwd 1",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );
}

void test_pwd_help(void)
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

        RUN_TEST(test_pwd_root);
        RUN_TEST(test_pwd_error);
        RUN_TEST(test_pwd_help);
        RUN_TEST(test_pwd_misc);

        return UNITY_END();
}
