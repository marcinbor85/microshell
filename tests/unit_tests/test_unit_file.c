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

#include "inc/ush_file.h"

int g_assert_call_count;

struct ush_object ush;
struct ush_descriptor ush_desc;

char *ush_node_get_absolute_path_in_path;
char *ush_node_get_absolute_path_out_path;
int ush_node_get_absolute_path_call_count;

char *ush_node_get_parent_by_path_path;
int ush_node_get_parent_by_path_call_count;
struct ush_node_object *ush_node_get_parent_by_path_return_val;

char *ush_utils_join_path_in_path;
char *ush_utils_join_path_name;
char *ush_utils_join_path_out_path;
int ush_utils_join_path_call_count;

struct ush_node_object node1;
const struct ush_file_descriptor cmd_files1[] = {
        {
                .name = "test"
        },
        {
                .name = "xyz"
        },
        {
                .name = "qwerty"
        },
};
struct ush_node_object node2;
const struct ush_file_descriptor cmd_files2[] = {
        {
                .name = "abc"
        },
        {
                .name = "def"
        },
        {
                .name = "ghi"
        },
};

void register_commands(void)
{
        node1.file_list = cmd_files1;
        node1.file_list_size = sizeof(cmd_files1) / sizeof(cmd_files1[0]);
        node1.path = "node1";
        node1.next = &node2;
        node2.file_list = cmd_files2;
        node2.file_list_size = sizeof(cmd_files2) / sizeof(cmd_files2[0]);
        node2.path = "node2";
        node2.next = NULL;

        ush.commands = &node1;
}

void register_files(void)
{
        node1.file_list = cmd_files1;
        node1.file_list_size = sizeof(cmd_files1) / sizeof(cmd_files1[0]);
        node1.next = &node2;
        node2.file_list = cmd_files2;
        node2.file_list_size = sizeof(cmd_files2) / sizeof(cmd_files2[0]);
        node2.next = NULL;

        ush.root = &node1;
}

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_node_get_absolute_path_in_path = NULL;
        ush_node_get_absolute_path_out_path = NULL;
        ush_node_get_absolute_path_call_count = 0;

        ush_node_get_parent_by_path_path = NULL;
        ush_node_get_parent_by_path_call_count = 0;
        ush_node_get_parent_by_path_return_val = NULL;

        ush_utils_join_path_in_path = NULL;
        ush_utils_join_path_name = NULL;
        ush_utils_join_path_out_path = NULL;
        ush_utils_join_path_call_count = 0;

        ush_desc.path_max_length = 256;
        ush.desc = &ush_desc;
}

void tearDown(void)
{

}

void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *out_path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_absolute_path_in_path, in_path);

        strcpy(out_path, ush_node_get_absolute_path_out_path);
        ush_node_get_absolute_path_call_count++;
}

struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_parent_by_path_path, path);

        ush_node_get_parent_by_path_call_count++;
        return ush_node_get_parent_by_path_return_val;
}

void ush_utils_join_path(const char *in_path, const char *name, char *out_path)
{
        strcpy(out_path, in_path);
        strcat(out_path, "/");
        strcat(out_path, name);

        ush_utils_join_path_call_count++;
}

void test_ush_file_find_by_path_not_exist(void)
{
        struct ush_file_descriptor const *ret;

        ush_node_get_absolute_path_in_path = "test_in";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_parent_by_path_path = "test_out";
        ret = ush_file_find_by_name(&ush, "test_in");
        TEST_ASSERT_NULL(ret);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_join_path_call_count);

        setUp();
        register_commands();

        ush_node_get_absolute_path_in_path = "test_in";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_parent_by_path_path = "test_out";
        ret = ush_file_find_by_name(&ush, "test_in");
        TEST_ASSERT_NULL(ret);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_join_path_call_count);

        setUp();
        register_files();

        ush_node_get_absolute_path_in_path = "test_in";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_parent_by_path_path = "test_out";
        ret = ush_file_find_by_name(&ush, "test_in");
        TEST_ASSERT_NULL(ret);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_join_path_call_count);
}

void test_ush_file_find_by_path_command_first(void)
{
        struct ush_file_descriptor const *ret;

        register_commands();
        ush_node_get_absolute_path_in_path = "test_in";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_parent_by_path_path = "test_out";
        ret = ush_file_find_by_name(&ush, "test");
        TEST_ASSERT_EQUAL(&cmd_files1[0], ret);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_join_path_call_count);
}

void test_ush_file_find_by_path_command_mid(void)
{
        struct ush_file_descriptor const *ret;

        register_commands();
        ush_node_get_absolute_path_in_path = "test_in";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_parent_by_path_path = "test_out";
        ret = ush_file_find_by_name(&ush, "qwerty");
        TEST_ASSERT_EQUAL(&cmd_files1[2], ret);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_join_path_call_count);
}

void test_ush_file_find_by_path_command_last(void)
{
        struct ush_file_descriptor const *ret;

        register_commands();
        ush_node_get_absolute_path_in_path = "test_in";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_parent_by_path_path = "test_out";
        ret = ush_file_find_by_name(&ush, "ghi");
        TEST_ASSERT_EQUAL(&cmd_files2[2], ret);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_join_path_call_count);
}

void test_ush_file_find_by_path_file_first(void)
{
        struct ush_file_descriptor const *ret;

        register_files();
        ush_node_get_absolute_path_in_path = "test";
        ush_node_get_absolute_path_out_path = "node1/test";
        ush_node_get_parent_by_path_path = "node1/test";
        ush_node_get_parent_by_path_return_val = &node1;
        ret = ush_file_find_by_name(&ush, "test");
        TEST_ASSERT_EQUAL(&cmd_files1[0], ret);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_utils_join_path_call_count);
}

void test_ush_file_find_by_path_file_mid(void)
{
        struct ush_file_descriptor const *ret;

        register_files();
        ush_node_get_absolute_path_in_path = "qwerty";
        ush_node_get_absolute_path_out_path = "node1/qwerty";
        ush_node_get_parent_by_path_path = "node1/qwerty";
        ush_node_get_parent_by_path_return_val = &node1;
        ret = ush_file_find_by_name(&ush, "qwerty");
        TEST_ASSERT_EQUAL(&cmd_files1[2], ret);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(3, ush_utils_join_path_call_count);
}

void test_ush_file_find_by_path_file_last(void)
{
        struct ush_file_descriptor const *ret;

        register_files();
        ush_node_get_absolute_path_in_path = "ghi";
        ush_node_get_absolute_path_out_path = "node2/ghi";
        ush_node_get_parent_by_path_path = "node2/ghi";
        ush_node_get_parent_by_path_return_val = &node1;
        ret = ush_file_find_by_name(&ush, "ghi");
        TEST_ASSERT_EQUAL(&cmd_files2[2], ret);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(6, ush_utils_join_path_call_count);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_file_find_by_path_not_exist);
        RUN_TEST(test_ush_file_find_by_path_command_first);
        RUN_TEST(test_ush_file_find_by_path_command_mid);
        RUN_TEST(test_ush_file_find_by_path_command_last);
        RUN_TEST(test_ush_file_find_by_path_file_first);
        RUN_TEST(test_ush_file_find_by_path_file_mid);
        RUN_TEST(test_ush_file_find_by_path_file_last);

        return UNITY_END();
}
