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

char* ush_print_buf;
int ush_print_call_count;

char* ush_file_find_by_name_name;
int ush_file_find_by_name_call_count;
struct ush_file_descriptor *ush_file_find_by_name_return_val;

struct ush_file_descriptor *set_data_file;
char *set_data_data;
size_t set_data_size;
int set_data_call_count;

extern void ush_buildin_cmd_echo_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush.state = USH_STATE__TOTAL_NUM;

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        ush_print_buf = NULL;
        ush_print_call_count = 0;

        ush_file_find_by_name_name = NULL;
        ush_file_find_by_name_call_count = 0;
        ush_file_find_by_name_return_val = NULL;

        set_data_file = NULL;
        set_data_data = NULL;
        set_data_size = 0;
        set_data_call_count = 0;
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

void ush_print(struct ush_object *self, char *buf)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_print_buf, buf);

        ush_print_call_count++;
}


struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_file_find_by_name_name, name);

        ush_file_find_by_name_call_count++;

        return ush_file_find_by_name_return_val;
}

void set_data(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(set_data_file, file);
        TEST_ASSERT_EQUAL(set_data_size, size);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(set_data_data, data, size);
        
        set_data_call_count++;
}

void test_ush_buildin_cmd_echo_callback_neg(void)
{
        char *argv[6] = {0};

        for (int i = 0; i < 6; i++) {
                setUp();

                if ((i == 1) || (i == 2) || (i == 4))
                        continue;

                ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                ush_buildin_cmd_echo_callback(&ush, NULL, i, argv);
                TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                TEST_ASSERT_EQUAL(0, ush_print_call_count);
                TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        }
}

void test_ush_buildin_cmd_echo_callback_neg_redirect(void)
{
        char *argv[6] = {0};
        for (int i = 0; i < 256; i++) {
                setUp();
                
                if (i == ((int)'>'))
                        continue;
                
                char data[2];
                data[0] = (char)i;
                data[1] = '\0';
                argv[2] = data;
                ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                ush_buildin_cmd_echo_callback(&ush, NULL, 4, argv);
                TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                TEST_ASSERT_EQUAL(0, ush_print_call_count);
                TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        }
}

void test_ush_buildin_cmd_echo_callback_neg_filenotfound(void)
{
        char *argv[6] = {0};
      
        argv[2] = ">";
        argv[3] = "test";

        ush_file_find_by_name_name = "test";
        ush_file_find_by_name_return_val = NULL;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_FOUND;
        ush_buildin_cmd_echo_callback(&ush, NULL, 4, argv);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
}

void test_ush_buildin_cmd_echo_callback_neg_not_writable(void)
{
        char *argv[6] = {0};
        struct ush_file_descriptor file = {0};
      
        argv[2] = ">";
        argv[3] = "test";

        file.set_data = NULL;

        ush_file_find_by_name_name = "test";
        ush_file_find_by_name_return_val = &file;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_WRITABLE;
        ush_buildin_cmd_echo_callback(&ush, NULL, 4, argv);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
}

void test_ush_buildin_cmd_echo_callback_pos(void)
{
        char *argv[6] = {0};
        struct ush_file_descriptor file = {0};
        char buf[128];

        ush_print_buf = NULL;
        ush_buildin_cmd_echo_callback(&ush, NULL, 1, argv);
        TEST_ASSERT_EQUAL(0, ush_print_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(USH_STATE_RESET, ush.state);

        setUp();

        sprintf(buf, "test");
        argv[1] = buf;
        ush_print_buf = "test";
        ush_buildin_cmd_echo_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_print_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);

        setUp();

        sprintf(buf, "abc\\x00""test");
        argv[1] = buf;
        ush_print_buf = "abc";
        ush_buildin_cmd_echo_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_print_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);

        setUp();

        sprintf(buf, "test2");
        argv[1] = buf;
        argv[2] = ">";
        argv[3] = "test_file";
        ush_file_find_by_name_name = "test_file";
        ush_file_find_by_name_return_val = &file;
        file.set_data = set_data;
        set_data_data = "test2";
        set_data_size = 5;
        set_data_file = &file;
        ush_buildin_cmd_echo_callback(&ush, NULL, 4, argv);
        TEST_ASSERT_EQUAL(0, ush_print_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, set_data_call_count);

        setUp();

        sprintf(buf, "a\\xA1\\x00""b");
        argv[1] = buf;
        argv[2] = ">";
        argv[3] = "test_file";
        ush_file_find_by_name_name = "test_file";
        ush_file_find_by_name_return_val = &file;
        file.set_data = set_data;
        set_data_data = "a\xA1\x00""b";
        set_data_size = 4;
        set_data_file = &file;
        ush_buildin_cmd_echo_callback(&ush, NULL, 4, argv);
        TEST_ASSERT_EQUAL(0, ush_print_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, set_data_call_count);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_echo_callback_neg);
        RUN_TEST(test_ush_buildin_cmd_echo_callback_neg_redirect);
        RUN_TEST(test_ush_buildin_cmd_echo_callback_neg_filenotfound);
        RUN_TEST(test_ush_buildin_cmd_echo_callback_neg_not_writable);
        RUN_TEST(test_ush_buildin_cmd_echo_callback_pos);

        return UNITY_END();
}
