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

void test_exec_ok(void)
{
        TEST_FUNC_ASK("test",
                "test_exec_callback\r\n"
                "[test /]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir/2/21");

        TEST_FUNC_ASK("../../../test",
                "test_exec_callback\r\n"
                "[test 21]$ "
        );
        TEST_FUNC_ASK("./../../../test",
                "test_exec_callback\r\n"
                "[test 21]$ "
        );
        TEST_FUNC_ASK("/test",
                "test_exec_callback\r\n"
                "[test 21]$ "
        );
}

void test_exec_error(void)
{
        TEST_FUNC_ASK("/data/binary",
                "error: file not executable\r\n"
                "[test /]$ "
        );
        TEST_FUNC_ASK("data/binary",
                "error: file not executable\r\n"
                "[test /]$ "
        );
        TEST_FUNC_ASK("data/bin",
                "error: syntax error\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_exec_ok);
        RUN_TEST(test_exec_error);

        return UNITY_END();
}
