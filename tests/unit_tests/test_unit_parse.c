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

#include "inc/ush_internal.h"
#include "inc/ush_file.h"

int g_assert_call_count;

struct ush_descriptor ush_desc;
struct ush_object ush;

int ush_parse_start_call_count;
int ush_parse_char_call_count;
int ush_parse_get_args_call_count;
int ush_file_find_by_name_call_count;
int ush_print_status_call_count;

int ush_parse_get_args_return_val;
ush_status_t ush_print_status_status;

int cmd_execute_callback_argc;
char *cmd_execute_callback_argv[4];
int cmd_execute_callback_call_count;
struct ush_file_descriptor *cmd_execute_callback_file;
int file_cmd_execute_callback_argc;
char *file_cmd_execute_callback_argv[4];
int file_cmd_execute_callback_call_count;
struct ush_file_descriptor *file_cmd_execute_callback_file;

char* ush_file_find_by_name_name;
struct ush_file_descriptor *ush_file_find_by_name_return_val;

struct ush_file_descriptor ush_file;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.path_max_length = 256;
        ush.desc = &ush_desc;

        memset((uint8_t*)&ush_file, 0, sizeof(struct ush_file_descriptor));

        ush_parse_start_call_count = 0;
        ush_parse_char_call_count = 0;
        ush_parse_get_args_call_count = 0;
        ush_file_find_by_name_call_count = 0;
        ush_print_status_call_count = 0;

        cmd_execute_callback_file = NULL;
        cmd_execute_callback_argc = 0;
        cmd_execute_callback_call_count = 0;
        memset((uint8_t*)cmd_execute_callback_argv, 0, sizeof(cmd_execute_callback_argv));

        file_cmd_execute_callback_file = NULL;
        file_cmd_execute_callback_argc = 0;
        file_cmd_execute_callback_call_count = 0;
        memset((uint8_t*)file_cmd_execute_callback_argv, 0, sizeof(file_cmd_execute_callback_argv));

        ush_parse_get_args_return_val = 0;
        ush_file_find_by_name_return_val = NULL;
        ush_file_find_by_name_name = NULL;
        ush_print_status_status = USH_STATUS__TOTAL_NUM;
}

void tearDown(void)
{

}

void cmd_execute_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        TEST_ASSERT_EQUAL(&ush, self);

        TEST_ASSERT_EQUAL(cmd_execute_callback_file, file);
        TEST_ASSERT_EQUAL(cmd_execute_callback_argc, argc);
        for (int i = 0; i < argc; i++)
                TEST_ASSERT_EQUAL_STRING(cmd_execute_callback_argv[i], argv[i]);
        TEST_ASSERT_NULL(argv[argc]);

        cmd_execute_callback_call_count++;
}

void file_cmd_execute_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        TEST_ASSERT_EQUAL(&ush, self);

        TEST_ASSERT_EQUAL(file_cmd_execute_callback_file, file);
        TEST_ASSERT_EQUAL(file_cmd_execute_callback_argc, argc);
        for (int i = 0; i < argc; i++)
                TEST_ASSERT_EQUAL_STRING(file_cmd_execute_callback_argv[i], argv[i]);
        TEST_ASSERT_NULL(argv[argc]);

        file_cmd_execute_callback_call_count++;
}

void ush_parse_start(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_parse_start_call_count++;
}

void ush_parse_char(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_parse_char_call_count++;
}

int ush_parse_get_args(struct ush_object *self, char* *argv)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_NOT_NULL(argv);

        argv[0] = ush_file_find_by_name_name;

        ush_parse_get_args_call_count++;

        return ush_parse_get_args_return_val;
}

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        TEST_ASSERT_EQUAL(&ush, self);

        TEST_ASSERT_EQUAL(ush_file_find_by_name_name, name);

        ush_file_find_by_name_call_count++;

        return ush_file_find_by_name_return_val;
}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_print_status_status, status);

        ush_print_status_call_count++;
}

void test_ush_parse_finish(void)
{
        setUp();
        ush.args_count = 0;
        ush_parse_get_args_return_val = 0;
        ush_parse_finish(&ush);
        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, cmd_execute_callback_call_count);
        TEST_ASSERT_EQUAL(0, file_cmd_execute_callback_call_count);

        setUp();
        ush.args_count = 1;
        ush_parse_get_args_return_val = 1;
        ush_file_find_by_name_name = (char*)1234;
        ush_file_find_by_name_return_val = NULL;
        ush_print_status_status = USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR;
        ush_parse_finish(&ush);
        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, cmd_execute_callback_call_count);
        TEST_ASSERT_EQUAL(0, file_cmd_execute_callback_call_count);

        setUp();
        ush.args_count = 1;
        ush_parse_get_args_return_val = 1;
        ush_file_find_by_name_name = "test";
        ush_desc.exec = cmd_execute_callback;
        cmd_execute_callback_argc = 1;
        cmd_execute_callback_argv[0] = "test";
        ush_file_find_by_name_return_val = NULL;
        ush_print_status_status = USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR;
        ush_parse_finish(&ush);
        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, cmd_execute_callback_call_count);
        TEST_ASSERT_EQUAL(0, file_cmd_execute_callback_call_count);

        setUp();
        ush.args_count = 1;
        ush_parse_get_args_return_val = 1;
        ush_file_find_by_name_name = "test";
        cmd_execute_callback_file = NULL;
        ush_desc.exec = cmd_execute_callback;
        cmd_execute_callback_argc = 1;
        cmd_execute_callback_argv[0] = "test";
        ush_file_find_by_name_return_val = &ush_file;
        ush_file.exec = NULL;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_EXECUTABLE;
        ush_parse_finish(&ush);
        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, cmd_execute_callback_call_count);
        TEST_ASSERT_EQUAL(0, file_cmd_execute_callback_call_count);

        setUp();
        ush.args_count = 1;
        ush_parse_get_args_return_val = 1;
        ush_file_find_by_name_name = "test";
        ush_desc.exec = cmd_execute_callback;
        cmd_execute_callback_argc = 1;
        cmd_execute_callback_argv[0] = "test";
        file_cmd_execute_callback_argc = 1;
        file_cmd_execute_callback_argv[0] = "test";
        ush_file_find_by_name_return_val = &ush_file;
        cmd_execute_callback_file = NULL;
        file_cmd_execute_callback_file = &ush_file;
        ush_file.exec = file_cmd_execute_callback;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_EXECUTABLE;
        ush_parse_finish(&ush);
        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, cmd_execute_callback_call_count);
        TEST_ASSERT_EQUAL(1, file_cmd_execute_callback_call_count);
}

void test_ush_parse_service(void)
{
       for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();

                ush_state_t state = (ush_state_t)i;
                ush.state = state;

                switch (state) {
                case USH_STATE_PARSE_PREPARE:
                        TEST_ASSERT_TRUE(ush_parse_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_parse_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        break;
                case USH_STATE_PARSE_SEARCH_ARG:
                case USH_STATE_PARSE_QUOTE_ARG:
                case USH_STATE_PARSE_STANDARD_ARG:
                        TEST_ASSERT_TRUE(ush_parse_service(&ush));
                        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_parse_char_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_parse_service(&ush));
                        TEST_ASSERT_EQUAL(0, ush_parse_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_char_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        break;
                }
       }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_parse_finish);
        RUN_TEST(test_ush_parse_service);

        return UNITY_END();
}
