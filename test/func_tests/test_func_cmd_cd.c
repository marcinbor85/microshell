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

void test_cmd_cd_abs(void)
{
        TEST_FUNC_ASK("cd /data",
                "[test data]$ "
        );

        TEST_FUNC_ASK("cd /dir/1",
                "[test 1]$ "
        );

        TEST_FUNC_ASK("cd /dir/2/21",
                "[test 21]$ "
        );

        TEST_FUNC_ASK("cd /dir/../data",
                "[test data]$ "
        );

        TEST_FUNC_ASK("cd /dir/../../../../../dir/1/12/../11",
                "[test 11]$ "
        );
}

void test_cmd_cd_rel(void)
{
        TEST_FUNC_ASK("cd data",
                "[test data]$ "
        );

        TEST_FUNC_ASK("cd .",
                "[test data]$ "
        );

        TEST_FUNC_ASK("cd ./../dir/2/21",
                "[test 21]$ "
        );

        TEST_FUNC_ASK("cd ../../../data",
                "[test data]$ "
        );

        TEST_FUNC_ASK("cd ../../../../../data",
                "[test data]$ "
        );

        TEST_FUNC_ASK("cd ././../../dir/1/12/../11",
                "[test 11]$ "
        );
}

void test_cmd_cd_error(void)
{
        TEST_FUNC_ASK("cd",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("cd 1 2",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );
}

void test_cmd_cd_help(void)
{
        TEST_FUNC_ASK("help cd",
                "cd: cd [path]\r\n"
                "\tChange current working directory.\r\n"
                "[test /]$ "
        );
}

void test_cmd_cd_not_exist(void)
{
        TEST_FUNC_ASK("cd test",
                "error: directory not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("cd di",
                "error: directory not found\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_cd_abs);
        RUN_TEST(test_cmd_cd_rel);
        RUN_TEST(test_cmd_cd_error);
        RUN_TEST(test_cmd_cd_help);
        RUN_TEST(test_cmd_cd_not_exist);

        return UNITY_END();
}
