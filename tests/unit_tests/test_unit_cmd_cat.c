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

char* ush_file_find_by_name_name[4];
int ush_file_find_by_name_call_count;
struct ush_file_descriptor *ush_file_find_by_name_return_val[4];
int ush_file_find_by_name_index;

struct ush_file_descriptor *get_data_file[4];
uint8_t *get_data_data[4];
size_t get_data_return_val[4];
int get_data_call_count;
int get_data_index;

char* ush_parse_get_args_argv[4];
int ush_parse_get_args_call_count;
int ush_parse_get_args_return_val;

struct ush_file_descriptor *ush_process_start_file;
int ush_process_start_call_count;

int ush_write_pointer_bin_call_count;
uint8_t *ush_write_pointer_bin_data;
size_t ush_write_pointer_bin_data_size;
ush_state_t ush_write_pointer_bin_write_next_state;

extern void ush_buildin_cmd_cat_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern bool ush_buildin_cmd_cat_service(struct ush_object *self, struct ush_file_descriptor const *file);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        memset((uint8_t*)ush_file_find_by_name_name, 0, sizeof(ush_file_find_by_name_name));
        memset((uint8_t*)ush_file_find_by_name_return_val, 0, sizeof(ush_file_find_by_name_return_val));
        ush_file_find_by_name_call_count = 0;
        ush_file_find_by_name_index = 0;

        memset((uint8_t*)get_data_file, 0, sizeof(get_data_file));
        memset((uint8_t*)get_data_return_val, 0, sizeof(get_data_return_val));
        memset((uint8_t*)get_data_data, 0, sizeof(get_data_data));
        get_data_call_count = 0;
        get_data_index = 0;

        memset((uint8_t*)ush_parse_get_args_argv, 0, sizeof(ush_parse_get_args_argv));
        ush_parse_get_args_call_count = 0;
        ush_parse_get_args_return_val = 0;

        ush_process_start_file = NULL;
        ush_process_start_call_count = 0;

        ush_write_pointer_bin_data = NULL;
        ush_write_pointer_bin_data_size = 0;
        ush_write_pointer_bin_write_next_state = USH_STATE__TOTAL_NUM;
        ush_write_pointer_bin_call_count = 0;
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

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_file_find_by_name_name[ush_file_find_by_name_index], name);

        ush_file_find_by_name_call_count++;

        return ush_file_find_by_name_return_val[ush_file_find_by_name_index++];
}

int ush_parse_get_args(struct ush_object *self, char* *argv)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_NOT_NULL(argv);

        for (int i = 0; i < ush_parse_get_args_return_val; i++)
                argv[i] = ush_parse_get_args_argv[i];
        ush_parse_get_args_call_count++;

        return ush_parse_get_args_return_val;
}

void ush_process_start(struct ush_object *self, const struct ush_file_descriptor *file)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_process_start_file, file);

        ush_process_start_call_count++;
}

void ush_write_pointer_bin(struct ush_object *self, uint8_t *data, size_t data_size, ush_state_t write_next_state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_NOT_NULL(data);

        TEST_ASSERT_EQUAL(ush_write_pointer_bin_data, data);
        TEST_ASSERT_EQUAL(ush_write_pointer_bin_data_size, data_size);
        TEST_ASSERT_EQUAL(ush_write_pointer_bin_write_next_state, write_next_state);
        
        ush_write_pointer_bin_call_count++;
}

size_t get_data(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(get_data_file[get_data_index], file);
        TEST_ASSERT_NOT_NULL(data);

        *data = get_data_data[get_data_index];
        get_data_call_count++;

        return get_data_return_val[get_data_index++];
}

void test_ush_buildin_cmd_cat_callback_neg_argc(void)
{
        char *argv[4] = {0};

        for (int i = 0; i < 2; i++) {
                setUp();

                ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                ush_buildin_cmd_cat_callback(&ush, NULL, i, argv);
                TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
                TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                TEST_ASSERT_EQUAL(0, get_data_call_count);
        }
}

void test_ush_buildin_cmd_cat_callback_neg_filenotfound(void)
{
        char *argv[4] = {0};
        struct ush_file_descriptor file0;

        setUp();

        argv[1] = "test";
        ush_file_find_by_name_name[0] = "test";
        ush_file_find_by_name_return_val[0] = NULL;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_FOUND;
        ush_buildin_cmd_cat_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, get_data_call_count);

        setUp();

        argv[1] = "test";
        argv[2] = "test2";
        ush_file_find_by_name_name[0] = "test";
        ush_file_find_by_name_name[1] = "test2";
        file0.get_data = get_data;
        ush_file_find_by_name_return_val[0] = &file0;
        ush_file_find_by_name_return_val[1] = NULL;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_FOUND;
        ush_buildin_cmd_cat_callback(&ush, NULL, 3, argv);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(2, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, get_data_call_count);
}

void test_ush_buildin_cmd_cat_callback_neg_filenodata(void)
{
        char *argv[4] = {0};
        struct ush_file_descriptor file0;
        struct ush_file_descriptor file1;

        setUp();

        argv[1] = "test";
        ush_file_find_by_name_name[0] = "test";
        file0.get_data = NULL;
        ush_file_find_by_name_return_val[0] = &file0;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_READABLE;
        ush_buildin_cmd_cat_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, get_data_call_count);

        setUp();

        argv[1] = "test";
        argv[2] = "test2";
        ush_file_find_by_name_name[0] = "test";
        ush_file_find_by_name_name[1] = "test2";
        file0.get_data = get_data;
        file1.get_data = NULL;
        ush_file_find_by_name_return_val[0] = &file0;
        ush_file_find_by_name_return_val[1] = &file1;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_READABLE;
        ush_buildin_cmd_cat_callback(&ush, NULL, 3, argv);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(2, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, get_data_call_count);
}

void test_ush_buildin_cmd_cat_callback_pos(void)
{
        char *argv[4] = {0};
        struct ush_file_descriptor file0;
        struct ush_file_descriptor file1;
        struct ush_file_descriptor cat_cmd = {0};

        argv[1] = "test";
        argv[2] = "test2";
        ush_file_find_by_name_name[0] = "test";
        ush_file_find_by_name_name[1] = "test2";
        file0.get_data = get_data;
        file1.get_data = get_data;
        ush_file_find_by_name_return_val[0] = &file0;
        ush_file_find_by_name_return_val[1] = &file1;
        ush_process_start_file = &cat_cmd;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_READABLE;
        ush_buildin_cmd_cat_callback(&ush, &cat_cmd, 3, argv);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(2, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(1, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, get_data_call_count);
}

void test_ush_buildin_cmd_cat_service_states(void)
{
        struct ush_file_descriptor cat_cmd;

        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.state = state;
                ush.process_index_item = 10;
                ush.args_count = 10;

                bool ret = ush_buildin_cmd_cat_service(&ush, &cat_cmd);
                TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_get_args_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
                TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                TEST_ASSERT_EQUAL(0, get_data_call_count);
                switch (state) {
                case USH_STATE_PROCESS_START:
                        TEST_ASSERT_EQUAL(1, ush.process_index_item);
                        TEST_ASSERT_EQUAL(USH_STATE_PROCESS_SERVICE, ush.state);
                        TEST_ASSERT_TRUE(ret);
                        break;
                case USH_STATE_PROCESS_SERVICE:
                        TEST_ASSERT_EQUAL(USH_STATE_PROCESS_FINISH, ush.state);
                        TEST_ASSERT_TRUE(ret);
                        break;
                case USH_STATE_PROCESS_FINISH:
                        TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
                        TEST_ASSERT_TRUE(ret);
                        break;
                default:
                        TEST_ASSERT_FALSE(ret);
                        break;
                }
        }
}

void test_ush_buildin_cmd_cat_service_process(void)
{
        struct ush_file_descriptor cat_cmd = {0};
        struct ush_file_descriptor file0;
        struct ush_file_descriptor file1;

        char *file_data = "file_data";
        char *file_data2 = "file_data_abc";

        ush.state = USH_STATE_PROCESS_SERVICE;
        ush.process_index_item = 0;
        ush.args_count = 1;
        ush_parse_get_args_argv[0] = "test";
        ush_parse_get_args_return_val = 1;
        file0.get_data = get_data;
        ush_file_find_by_name_return_val[0] = &file0;
        ush_file_find_by_name_name[0] = "test";
        get_data_file[0] = &file0;
        get_data_data[0] = (uint8_t*)file_data;
        get_data_return_val[0] = 9;
        ush_write_pointer_bin_data = (uint8_t*)file_data;
        ush_write_pointer_bin_data_size = 9;
        ush_write_pointer_bin_write_next_state = USH_STATE_PROCESS_SERVICE;

        TEST_ASSERT_TRUE(ush_buildin_cmd_cat_service(&ush, &cat_cmd));
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(1, get_data_call_count);
        TEST_ASSERT_EQUAL(USH_STATE_PROCESS_SERVICE, ush.state);
        TEST_ASSERT_EQUAL(1, ush.process_index_item);

        setUp();

        ush.state = USH_STATE_PROCESS_SERVICE;
        ush.process_index_item = 1;
        ush.args_count = 2;
        ush_parse_get_args_argv[0] = "test";
        ush_parse_get_args_argv[1] = "test2";
        ush_parse_get_args_return_val = 2;
        file0.get_data = get_data;
        file1.get_data = get_data;
        ush_file_find_by_name_index = 1;
        ush_file_find_by_name_return_val[0] = &file0;
        ush_file_find_by_name_return_val[1] = &file1;
        ush_file_find_by_name_name[0] = "test";
        ush_file_find_by_name_name[1] = "test2";
        get_data_index = 1;
        get_data_file[0] = &file0;
        get_data_file[1] = &file1;
        get_data_data[0] = (uint8_t*)file_data;
        get_data_data[1] = (uint8_t*)file_data2;
        get_data_return_val[0] = 9;
        get_data_return_val[1] = 13;
        ush_write_pointer_bin_data = (uint8_t*)file_data2;
        ush_write_pointer_bin_data_size = 13;
        ush_write_pointer_bin_write_next_state = USH_STATE_PROCESS_SERVICE;

        TEST_ASSERT_TRUE(ush_buildin_cmd_cat_service(&ush, &cat_cmd));
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_get_args_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_bin_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(1, get_data_call_count);
        TEST_ASSERT_EQUAL(USH_STATE_PROCESS_SERVICE, ush.state);
        TEST_ASSERT_EQUAL(2, ush.process_index_item);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_cat_callback_neg_argc);
        RUN_TEST(test_ush_buildin_cmd_cat_callback_neg_filenotfound);
        RUN_TEST(test_ush_buildin_cmd_cat_callback_neg_filenodata);
        RUN_TEST(test_ush_buildin_cmd_cat_callback_pos);
        RUN_TEST(test_ush_buildin_cmd_cat_service_states);
        RUN_TEST(test_ush_buildin_cmd_cat_service_process);

        return UNITY_END();
}
