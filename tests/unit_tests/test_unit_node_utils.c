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
#include "inc/ush_const.h"
#include "inc/ush.h"

int g_assert_call_count;

struct ush_object ush;
struct ush_descriptor ush_desc;

int ush_utils_path_upper_call_count;
char *ush_utils_path_upper_path_in;
char *ush_utils_path_upper_path_out;

int ush_node_get_by_path_call_count;
char *ush_node_get_by_path_path_in;
struct ush_node_object *ush_node_get_by_path_return_val;

char *ush_node_get_absolute_path_in_path;
char *ush_node_get_absolute_path_out_path;
int ush_node_get_absolute_path_call_count;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_utils_path_upper_call_count = 0;
        ush_utils_path_upper_path_in = NULL;
        ush_utils_path_upper_path_out = NULL;

        ush_node_get_by_path_call_count = 0;
        ush_node_get_by_path_path_in = NULL;
        ush_node_get_by_path_return_val = NULL;

        ush_node_get_absolute_path_in_path = NULL;
        ush_node_get_absolute_path_out_path = NULL;
        ush_node_get_absolute_path_call_count = 0;

        ush_desc.path_max_length = 256;
        ush.desc = &ush_desc;
}

void tearDown(void)
{

}

void ush_utils_path_upper(char *path)
{
        TEST_ASSERT_EQUAL_STRING(ush_utils_path_upper_path_in, path);

        strcpy(path, ush_utils_path_upper_path_out);

        ush_utils_path_upper_call_count++;
}

struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_by_path_path_in, path);

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

void test_ush_node_get_parent_by_path(void)
{
        struct ush_node_object *ret;

        ush_utils_path_upper_path_in = "test";
        ush_utils_path_upper_path_out = "test_out";
        ush_node_get_by_path_path_in = "test_out";
        ush_node_get_by_path_return_val = (struct ush_node_object*)1234;
        ret = ush_node_get_parent_by_path(&ush, "test");
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ret);
        TEST_ASSERT_EQUAL(1, ush_utils_path_upper_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);

        setUp();

        ush_utils_path_upper_path_in = "test2";
        ush_utils_path_upper_path_out = "";
        ret = ush_node_get_parent_by_path(&ush, "test2");
        TEST_ASSERT_NULL(ret);
        TEST_ASSERT_EQUAL(1, ush_utils_path_upper_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
}

void test_ush_node_set_current_dir(void)
{
        ush.current_node = (struct ush_node_object*)1234;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_set_current_dir(&ush, ""));
        TEST_ASSERT_EQUAL(0, ush_utils_path_upper_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.current_node);

        setUp();

        ush.current_node = (struct ush_node_object*)1234;
        ush_node_get_absolute_path_in_path = "test";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_by_path_path_in = "test_out";
        ush_node_get_by_path_return_val = (struct ush_node_object*)5678;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&ush, "test"));
        TEST_ASSERT_EQUAL(0, ush_utils_path_upper_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL((struct ush_node_object*)5678, ush.current_node);

        setUp();

        ush.current_node = (struct ush_node_object*)1234;
        ush_node_get_absolute_path_in_path = "test";
        ush_node_get_absolute_path_out_path = "test_out";
        ush_node_get_by_path_path_in = "test_out";
        ush_node_get_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_set_current_dir(&ush, "test"));
        TEST_ASSERT_EQUAL(0, ush_utils_path_upper_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.current_node);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_node_get_parent_by_path);
        RUN_TEST(test_ush_node_set_current_dir);

        return UNITY_END();
}
