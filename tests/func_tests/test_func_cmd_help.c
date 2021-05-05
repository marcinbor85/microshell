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

void test_cmd_help_self(void)
{
        TEST_FUNC_ASK("help",
                "help             - list available commands\r\n"
                "ls               - list directory content\r\n"
                "cd               - change current directory\r\n"
                "pwd              - print current directory\r\n"
                "cat              - print files content\r\n"
                "xxd              - dump file hex content\r\n"
                "echo             - print string to file\r\n"
                "[test /]$ "
        );
}

void test_cmd_help_help(void)
{
        TEST_FUNC_ASK("help help",
                "help: help [file]\r\n"
                "\tShow help information for file or command.\r\n"
                "      help\r\n"
                "\tShow available commands.\r\n"
                "[test /]$ "
        );
}

void test_cmd_help_error(void)
{
        TEST_FUNC_ASK("help help abc",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("help hel",
                "error: file not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("help /data/binary",
                "error: no help available\r\n"
                "[test /]$ "
        );
}

void test_cmd_help_file(void)
{
        TEST_FUNC_ASK("help ./test",
                "nothing special\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_help_self);
        RUN_TEST(test_cmd_help_help);
        RUN_TEST(test_cmd_help_error);
        RUN_TEST(test_cmd_help_file);

        return UNITY_END();
}
