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

struct ush_node_object root;
struct ush_node_object node1;
struct ush_node_object node11;
struct ush_node_object node12;
struct ush_node_object node121;
struct ush_node_object node2;
struct ush_node_object node21;
struct ush_node_object node3;
struct ush_node_object node31;
struct ush_node_object node311;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        memset((uint8_t*)&root, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node1, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node11, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node12, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node121, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node2, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node21, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node3, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node31, 0, sizeof(struct ush_node_object));
        memset((uint8_t*)&node311, 0, sizeof(struct ush_node_object));
        
        ush.root = &root;
        ush_desc.path_max_length = 256;
        ush.desc = &ush_desc;
        root.path = "/";

        root.children = &node1;
        node1.next = &node2;
        node1.parent = &root;
        node1.path = "/1";
        node2.next = &node3;
        node2.parent = &root;
        node2.path = "/2";
        node3.parent = &root;
        node3.path = "/3";

        node1.children = &node11;
        node11.next = &node12;
        node11.parent = &node1;
        node11.path = "/1/1";

        node12.children = &node121;
        node12.path = "/1/2";
        node121.parent = &node12;
        node121.path = "/1/2/1";

        node2.children = &node21;
        node2.path = "/2";
        node21.parent = &node2;
        node21.path = "/2/1";

        node3.children = &node31;
        node3.path = "/3";
        node31.parent = &node3;
        node31.path = "/3/1";
        node31.children = &node311;
        node311.parent = &node31;
        node311.path = "/3/1/1";
}

void tearDown(void)
{

}

void test_ush_node_get_by_path_null(void)
{
        struct ush_node_object node = {0};

        setUp();
        ush.root = NULL;
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "/1"));

        setUp();
        ush.root = &node;
        node.children = NULL;
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "/1"));
}

void test_ush_node_get_by_path_exist(void)
{
        TEST_ASSERT_EQUAL(&root, ush_node_get_by_path(&ush, ""));
        TEST_ASSERT_EQUAL(&root, ush_node_get_by_path(&ush, "/"));
        TEST_ASSERT_EQUAL(&node1, ush_node_get_by_path(&ush, "/1"));
        TEST_ASSERT_EQUAL(&node2, ush_node_get_by_path(&ush, "/2"));
        TEST_ASSERT_EQUAL(&node3, ush_node_get_by_path(&ush, "/3"));
        TEST_ASSERT_EQUAL(&node11, ush_node_get_by_path(&ush, "/1/1"));
        TEST_ASSERT_EQUAL(&node12, ush_node_get_by_path(&ush, "/1/2"));
        TEST_ASSERT_EQUAL(&node121, ush_node_get_by_path(&ush, "/1/2/1"));
        TEST_ASSERT_EQUAL(&node2, ush_node_get_by_path(&ush, "/2"));
        TEST_ASSERT_EQUAL(&node21, ush_node_get_by_path(&ush, "/2/1"));
        TEST_ASSERT_EQUAL(&node3, ush_node_get_by_path(&ush, "/3"));
        TEST_ASSERT_EQUAL(&node31, ush_node_get_by_path(&ush, "/3/1"));
        TEST_ASSERT_EQUAL(&node311, ush_node_get_by_path(&ush, "/3/1/1"));
}

void test_ush_node_get_by_path_not_exist(void)
{
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "/ 1"));
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "."));
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "test"));
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "/121"));
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "/1/2/3"));
        TEST_ASSERT_NULL(ush_node_get_by_path(&ush, "/1/2/"));
}

#define TEST_STRING_PROCESS_ARGS(expected, arg1, input, func) { \
        memset(out, 0, sizeof(out)); \
        strcpy(in, input); \
        func(arg1, in, out); \
        TEST_ASSERT_EQUAL_STRING(expected, out); \
}

void test_ush_node_get_absolute_path(void)
{
        char in[128];
        char out[128];

        TEST_STRING_PROCESS_ARGS("/", &ush, "/", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "/.", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "/..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "/../", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1", &ush, "/../1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/test", &ush, "/../test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1", &ush, "/../3/1/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/2/1", &ush, "/2/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/2/test", &ush, "/./1/2/test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "/../../../../..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/root", &ush, "/../../../../../root", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/9/8/7", &ush, "/9/8/7", ush_node_get_absolute_path);

        ush.current_node = &root;
        TEST_STRING_PROCESS_ARGS("/", &ush, "", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, ".", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "../", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1", &ush, "../1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/test", &ush, "../test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1", &ush, "../3/1/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/2/1", &ush, "2/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/2/test", &ush, "./1/2/test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "../../../../..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/root", &ush, "../../../../../root", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/9/8/7", &ush, "9/8/7", ush_node_get_absolute_path);

        ush.current_node = &node311;
        TEST_STRING_PROCESS_ARGS("/3/1/1", &ush, "", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1", &ush, ".", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1", &ush, "..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1", &ush, "../", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1", &ush, "../1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/test", &ush, "../test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/3/1/1", &ush, "../3/1/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1/2/1", &ush, "2/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1/1/2/test", &ush, "./1/2/test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "../../../../..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/root", &ush, "../../../../../root", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/3/1/1/9/8/7", &ush, "9/8/7", ush_node_get_absolute_path);

        ush.current_node = &node12;
        TEST_STRING_PROCESS_ARGS("/1/2", &ush, "", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/2", &ush, ".", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1", &ush, "..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1", &ush, "../", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/1", &ush, "../1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/test", &ush, "../test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/3/1/1", &ush, "../3/1/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/2/2/1", &ush, "2/1", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/2/1/2/test", &ush, "./1/2/test", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/", &ush, "../../../../..", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/root", &ush, "../../../../../root", ush_node_get_absolute_path);
        TEST_STRING_PROCESS_ARGS("/1/2/9/8/7", &ush, "9/8/7", ush_node_get_absolute_path);
}

void test_ush_node_deinit_recursive(void)
{
        struct ush_node_object node = {0};
        struct ush_node_object node2 = {0};
        struct ush_node_object node3 = {0};

        memset((uint8_t*)&node, 0xFF, sizeof(struct ush_node_object));
        node.children = NULL;
        node.next = &node2;
        memset((uint8_t*)&node2, 0xFF, sizeof(struct ush_node_object));
        node2.children = &node3;
        node2.next = NULL;
        memset((uint8_t*)&node3, 0xFF, sizeof(struct ush_node_object));
        node3.children = NULL;
        node3.next = NULL;

        ush_node_deinit_recursive(&ush, &node);
        TEST_ASSERT_EACH_EQUAL_UINT8(0, (uint8_t*)&node, sizeof(struct ush_node_object));
        TEST_ASSERT_EACH_EQUAL_UINT8(0, (uint8_t*)&node2, sizeof(struct ush_node_object));
        TEST_ASSERT_EACH_EQUAL_UINT8(0, (uint8_t*)&node3, sizeof(struct ush_node_object));
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_node_get_by_path_null);
        RUN_TEST(test_ush_node_get_by_path_exist);
        RUN_TEST(test_ush_node_get_by_path_not_exist);
        RUN_TEST(test_ush_node_get_absolute_path);
        RUN_TEST(test_ush_node_deinit_recursive);

        return UNITY_END();
}
