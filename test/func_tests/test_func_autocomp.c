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

void test_autocomp_full(void)
{
        test_func_write("\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "\r\n"
                "help\r\n"
                "ls\r\n"
                "cd\r\n"
                "pwd\r\n"
                "cat\r\n"
                "xxd\r\n"
                "echo\r\n"
                "test\r\n"
                "dir\r\n"
                "data\r\n"
                "[test /]$ ",
                g_write_buf
        );
}

void test_autocomp_part(void)
{
        test_func_write("d\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "d\r\n"
                "dir\r\n"
                "data\r\n"
                "[test /]$ d",
                g_write_buf
        );
}

void test_autocomp_finish(void)
{
        test_func_write("da\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "data",
                g_write_buf
        );

        setUp();

        test_func_write("h\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "help",
                g_write_buf
        );

        setUp();

        test_func_write("di\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "dir",
                g_write_buf
        );

        setUp();

        test_func_write("p\t\n");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "pwd\n"
                "/\r\n"
                "[test /]$ ",
                g_write_buf
        );
}

void test_autocomp_unknown(void)
{
        test_func_write("q\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "q",
                g_write_buf
        );
}

void test_autocomp_args(void)
{
        test_func_write("cd d\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "cd d\r\n"
                "dir\r\n"
                "data\r\n"
                "[test /]$ cd d",
                g_write_buf
        );

        test_func_write("i\t\n");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "ir\n"
                "[test dir]$ ",
                g_write_buf
        );

        test_func_write("\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "\r\n"
                "help\r\n"
                "ls\r\n"
                "cd\r\n"
                "pwd\r\n"
                "cat\r\n"
                "xxd\r\n"
                "echo\r\n"
                "2\r\n"
                "1\r\n"
                "[test dir]$ ",
                g_write_buf
        );

        test_func_write("pwd pwd   pw\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "pwd pwd   pwd",
                g_write_buf
        );
        
        test_func_write("\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "",
                g_write_buf
        );

        test_func_write(" \t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                " \r\n"
                "help\r\n"
                "ls\r\n"
                "cd\r\n"
                "pwd\r\n"
                "cat\r\n"
                "xxd\r\n"
                "echo\r\n"
                "2\r\n"
                "1\r\n"
                "[test dir]$ pwd pwd   pwd ",
                g_write_buf
        );
}

void test_autocomp_complex(void)
{
        test_func_ask(
                "cd data",
                "[test data]$ "
        );

        test_func_write("te\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "text",
                g_write_buf
        );

        test_func_write("\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "\r\n"
                "text\r\n"
                "text_file1\r\n"
                "text_file2\r\n"
                "[test data]$ text",
                g_write_buf
        );

        test_func_write("_\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "_file",
                g_write_buf
        );

        test_func_write("\t");
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(
                "\r\n"
                "text_file1\r\n"
                "text_file2\r\n"
                "[test data]$ text_file",
                g_write_buf
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_autocomp_full);
        RUN_TEST(test_autocomp_part);
        RUN_TEST(test_autocomp_finish);
        RUN_TEST(test_autocomp_unknown);
        RUN_TEST(test_autocomp_args);
        RUN_TEST(test_autocomp_complex);

        return UNITY_END();
}
