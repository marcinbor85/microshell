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

        TEST_FUNC_ASK("cd test",
                "error: directory not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("cd di",
                "error: directory not found\r\n"
                "[test /]$ "
        );
}

void test_cmd_cd_help(void)
{
        TEST_FUNC_ASK("help cd",
                "usage: cd <path>\r\n"
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

        return UNITY_END();
}
