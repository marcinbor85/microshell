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
#include <string.h>

#include <unity.h>

#include "inc/ush_commands.h"

int g_assert_call_count;

struct ush_object ush;

ush_status_t ush_print_status_status;
int ush_print_status_call_count;

char *ush_node_set_current_dir_path;
int ush_node_set_current_dir_call_count;
ush_status_t ush_node_set_current_dir_return_val;

extern void ush_buildin_cmd_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        ush_node_set_current_dir_path = NULL;
        ush_node_set_current_dir_call_count = 0;
        ush_node_set_current_dir_return_val = USH_STATUS__TOTAL_NUM;
}

void tearDown(void)
{

}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_print_status_status, status);

        ush_print_status_call_count++;
}

ush_status_t ush_node_set_current_dir(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_set_current_dir_path, path);

        ush_node_set_current_dir_call_count++;
        
        return ush_node_set_current_dir_return_val;
}

void test_ush_buildin_cmd_cd_callback_neg(void)
{
        for (int i = 0; i < 6; i++) {
                setUp();
                if (i != 2) {
                        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                        ush_buildin_cmd_cd_callback(&ush, NULL, i, NULL);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_set_current_dir_call_count);
                }
        }
}

void test_ush_buildin_cmd_cd_callback_pos(void)
{
        for (int n = 0; n < USH_STATUS__TOTAL_NUM; n++) {
                setUp();

                ush_status_t status = (ush_status_t)n;
                char *argv[2] = {0};
                
                argv[1] = "test";
                ush_node_set_current_dir_return_val = status;
                ush_node_set_current_dir_path = "test";
                ush_print_status_status = status;
                ush_buildin_cmd_cd_callback(&ush, NULL, 2, argv);
                TEST_ASSERT_EQUAL(1, ush_node_set_current_dir_call_count);
                if (status != USH_STATUS_OK) {
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                } else {
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_cd_callback_neg);
        RUN_TEST(test_ush_buildin_cmd_cd_callback_pos);

        return UNITY_END();
}
