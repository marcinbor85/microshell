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

void test_input_empty(void)
{
        test_func_ask("",
                "[test /]$ "
        );
}

void test_input_error(void)
{
        test_func_ask("abcd",
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
