/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
                "usage: help [file]\r\n"
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
