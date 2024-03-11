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
#include "inc/ush_shell.h"

int g_assert_call_count;

char output_buf[256];
struct ush_object ush;
struct ush_descriptor ush_desc;

ush_status_t ush_print_status_status;
int ush_print_status_call_count;

struct ush_file_descriptor *ush_process_start_file;
int ush_process_start_call_count;

int ush_write_pointer_call_count;
char *ush_write_pointer_text;
ush_state_t ush_write_pointer_state;

char *ush_node_get_absolute_path_in_path;
char *ush_node_get_absolute_path_out_path;
int ush_node_get_absolute_path_call_count;

char *ush_node_get_by_path_path;
int ush_node_get_by_path_call_count;
struct ush_node_object *ush_node_get_by_path_return_val;

extern void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern bool ush_buildin_cmd_ls_service(struct ush_object *self, struct ush_file_descriptor const *file);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.output_buffer = output_buf;
        ush_desc.output_buffer_size = sizeof(output_buf);
        ush_desc.path_max_length = 256;

        ush.desc = &ush_desc;

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        ush_process_start_file = NULL;
        ush_process_start_call_count = 0;

        ush_write_pointer_call_count = 0;
        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;

        ush_node_get_absolute_path_in_path = NULL;
        ush_node_get_absolute_path_out_path = NULL;
        ush_node_get_absolute_path_call_count = 0;

        ush_node_get_by_path_path = NULL;
        ush_node_get_by_path_call_count = 0;
        ush_node_get_by_path_return_val = NULL;

        g_assert_call_count = 0;
}

void tearDown(void)
{

}

struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_by_path_path, path);

        ush_node_get_by_path_call_count++;

        return ush_node_get_by_path_return_val;
}

void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *out_path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_absolute_path_in_path, in_path);

        strcpy(out_path, ush_node_get_absolute_path_out_path);
        ush_node_get_absolute_path_call_count++;
}

void ush_process_start(struct ush_object *self, const struct ush_file_descriptor *file)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_process_start_file, file);

        ush_process_start_call_count++;
}

void ush_print_status(struct ush_object *self, ush_status_t status)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_print_status_status, status);

        ush_print_status_call_count++;
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_write_pointer_text, text);
        TEST_ASSERT_EQUAL(ush_write_pointer_state, state);

        ush_write_pointer_call_count++;
}

void test_ush_buildin_cmd_ls_callback(void)
{
        char *argv[4] = {0};

        for (int i = 0; i < 10; i++) {
                setUp();

                switch (i) {
                case 1:
                        ush.current_node = (struct ush_node_object*)1234;
                        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                        ush_process_start_file =  (struct ush_file_descriptor*)5678;
                        ush_buildin_cmd_ls_callback(&ush, (struct ush_file_descriptor*)5678, i, NULL);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(1, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        break;
                case 2:
                        setUp();
                        ush.process_node = (struct ush_node_object*)1234;
                        argv[1] = "abc";
                        ush_node_get_absolute_path_out_path = "test";
                        ush_node_get_absolute_path_in_path = "abc";
                        ush_node_get_by_path_path = "test";
                        ush_node_get_by_path_return_val = NULL;
                        ush_print_status_status = USH_STATUS_ERROR_NODE_NOT_FOUND;
                        ush_buildin_cmd_ls_callback(&ush, (struct ush_file_descriptor*)5678, i, argv);
                        TEST_ASSERT_NULL(ush.process_node);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);

                        setUp();
                        argv[1] = "abcd";
                        ush_node_get_absolute_path_out_path = "test2";
                        ush_node_get_absolute_path_in_path = "abcd";
                        ush_node_get_by_path_path = "test2";
                        ush_node_get_by_path_return_val = (struct ush_node_object*)1234;
                        ush_process_start_file =  (struct ush_file_descriptor*)2345;
                        ush_buildin_cmd_ls_callback(&ush, (struct ush_file_descriptor*)2345, i, argv);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(1, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
                        break;
                default:
                        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                        ush_buildin_cmd_ls_callback(&ush, NULL, i, NULL);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        break;
                }
        }
}

void test_ush_buildin_cmd_ls_service_start(void)
{
        struct ush_file_descriptor file = {0};
        struct ush_node_object node = {0};

        setUp();
        ush.state = USH_STATE_PROCESS_START;
        ush.process_node = &node;
        ush.process_index = 10;
        node.parent = NULL;
        node.children = (struct ush_node_object*)1234;
        ush_write_pointer_text = "d---- " USH_SHELL_FONT_COLOR_GREEN "." USH_SHELL_FONT_STYLE_RESET "\r\n";
        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_child_node);
        TEST_ASSERT_EQUAL(0, ush.process_index);

        setUp();
        ush.state = USH_STATE_PROCESS_START;
        ush.process_node = &node;
        ush.process_index = 10;
        node.parent = (struct ush_node_object*)5678;
        node.children = (struct ush_node_object*)1234;
        ush_write_pointer_text =
                "d---- " USH_SHELL_FONT_COLOR_GREEN "." USH_SHELL_FONT_STYLE_RESET "\r\n"
                "d---- " USH_SHELL_FONT_COLOR_GREEN ".." USH_SHELL_FONT_STYLE_RESET "\r\n";
        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_child_node);
        TEST_ASSERT_EQUAL(0, ush.process_index);
}

void test_ush_buildin_cmd_ls_service_service(void)
{
        struct ush_file_descriptor file = {0};
        struct ush_node_object node = {0};

        for (int i = 0; i < 10; i++) {
                setUp();
                ush.state = USH_STATE_PROCESS_SERVICE;
                ush.process_child_node = NULL;
                ush.process_index = i;
                ush.process_index_item = 10;
                TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                TEST_ASSERT_EQUAL(0, g_assert_call_count);
                TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                TEST_ASSERT_EQUAL(0, ush.process_index_item);
                TEST_ASSERT_EQUAL(0, ush.process_index);
                TEST_ASSERT_EQUAL(USH_STATE_PROCESS_FINISH, ush.state);

                setUp();
                ush.state = USH_STATE_PROCESS_SERVICE;
                node.next = (struct ush_node_object*)1234;
                node.path = "/test/abc";
                ush.process_child_node = &node;
                ush.process_index = i;
                ush.process_index_item = 10;

                switch (i) {
                case 0:
                        ush_write_pointer_text = "d---- " USH_SHELL_FONT_COLOR_GREEN;
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(10, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_index);
                        break;
                case 1:
                        ush_write_pointer_text = "abc";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(10, ush.process_index_item);
                        TEST_ASSERT_EQUAL(2, ush.process_index);
                        break;
                case 2:
                        ush_write_pointer_text = "/" USH_SHELL_FONT_STYLE_RESET "\r\n";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(10, ush.process_index_item);
                        TEST_ASSERT_EQUAL(3, ush.process_index);
                        break;
                case 3:
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(10, ush.process_index_item);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_child_node);
                        break;
                default:
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(1, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        break;
                }
        }
}

void test_ush_buildin_cmd_ls_service_finish(void)
{
        struct ush_file_descriptor file = {0};
        struct ush_node_object node = {0};
        struct ush_file_descriptor files[2] = {
                {
                        .name = "test",
                        .description = "test_desc",
                },
                {
                        .name = "qwerty",
                        .description = NULL,
                }
        };
        node.file_list = files;
        node.file_list_size = 2;

        for (int i = 0; i < 10; i++) {
                setUp();
                ush.state = USH_STATE_PROCESS_FINISH;
                ush.process_node = &node;
                ush.process_index = i;
                ush.process_index_item = 2;
                TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                TEST_ASSERT_EQUAL(0, g_assert_call_count);
                TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                TEST_ASSERT_EQUAL(2, ush.process_index_item);
                TEST_ASSERT_EQUAL(i, ush.process_index);
                TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
                
                setUp();
                ush.state = USH_STATE_PROCESS_FINISH;
                ush.process_node = &node;
                ush.process_index = i;
                ush.process_index_item = 0;

                switch (i) {
                case 0:
                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        files[0].get_data = (ush_file_data_getter)1;
                        files[0].set_data = (ush_file_data_setter)1;
                        files[0].exec = (ush_file_execute_callback)1;
                        files[0].help = (const char*)1;
                        ush_write_pointer_text = "-rwxh ";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("-rwxh ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        files[0].get_data = NULL;
                        files[0].set_data = (ush_file_data_setter)1;
                        files[0].exec = (ush_file_execute_callback)1;
                        files[0].help = (const char*)1;
                        ush_write_pointer_text = "--wxh ";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("--wxh ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        files[0].get_data = NULL;
                        files[0].set_data = NULL;
                        files[0].exec = (ush_file_execute_callback)1;
                        files[0].help = (const char*)1;
                        ush_write_pointer_text = "---xh ";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("---xh ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        files[0].get_data = NULL;
                        files[0].set_data = NULL;
                        files[0].exec = NULL;
                        files[0].help = (const char*)1;
                        ush_write_pointer_text = "----h ";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("----h ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        files[0].get_data = NULL;
                        files[0].set_data = NULL;
                        files[0].exec = NULL;
                        files[0].help = NULL;
                        ush_write_pointer_text = "----- ";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("----- ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_index);
                        break;
                case 1:
                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        ush_write_pointer_text = "test             " USH_SHELL_FONT_COLOR_YELLOW "- ";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("test             " USH_SHELL_FONT_COLOR_YELLOW "- ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(2, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 1;
                        ush_write_pointer_text = "qwerty";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("qwerty", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(1, ush.process_index_item);
                        TEST_ASSERT_EQUAL(3, ush.process_index);
                        break;
                case 2:
                        ush_write_pointer_text = "test_desc";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(3, ush.process_index);
                        break;
                case 3:
                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 0;
                        ush_write_pointer_text = USH_SHELL_FONT_STYLE_RESET "\r\n";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(1, ush.process_index_item);
                        TEST_ASSERT_EQUAL(0, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_FINISH;
                        ush.process_node = &node;
                        ush.process_index = i;
                        ush.process_index_item = 1;
                        ush_write_pointer_text = "\r\n";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        TEST_ASSERT_EQUAL(2, ush.process_index_item);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        break;
                default:
                        TEST_ASSERT_TRUE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(1, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        break;
                }
        }
}

void test_ush_buildin_cmd_ls_service_states(void)
{
        struct ush_file_descriptor file = {0};

        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.state = i;

                switch (state) {
                case USH_STATE_PROCESS_START:
                case USH_STATE_PROCESS_SERVICE:
                case USH_STATE_PROCESS_FINISH:
                        continue;
                default:
                        TEST_ASSERT_FALSE(ush_buildin_cmd_ls_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_ls_callback);
        RUN_TEST(test_ush_buildin_cmd_ls_service_start);
        RUN_TEST(test_ush_buildin_cmd_ls_service_service);
        RUN_TEST(test_ush_buildin_cmd_ls_service_finish);
        RUN_TEST(test_ush_buildin_cmd_ls_service_states);

        return UNITY_END();
}
