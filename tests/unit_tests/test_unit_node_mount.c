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

char *ush_node_get_by_path_path;
int ush_node_get_by_path_call_count;
struct ush_node_object *ush_node_get_by_path_return_val;

char *ush_node_get_parent_by_path_path;
int ush_node_get_parent_by_path_call_count;
struct ush_node_object *ush_node_get_parent_by_path_return_val;

struct ush_node_object node1;
struct ush_node_object node2;
struct ush_node_object node3;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_node_get_by_path_path = NULL;
        ush_node_get_by_path_call_count = 0;
        ush_node_get_by_path_return_val = NULL;

        ush_node_get_parent_by_path_path = NULL;
        ush_node_get_parent_by_path_call_count = 0;
        ush_node_get_parent_by_path_return_val = NULL;

        memset((uint8_t*)&node1, 0, sizeof(node1));
        memset((uint8_t*)&node2, 0, sizeof(node2));
        memset((uint8_t*)&node3, 0, sizeof(node3));
}

void tearDown(void)
{

}

struct ush_node_object* ush_node_get_by_path(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_node_get_by_path_path, path);

        ush_node_get_by_path_call_count++;

        return ush_node_get_by_path_return_val;
}

struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_node_get_parent_by_path_path, path);

        ush_node_get_parent_by_path_call_count++;

        return ush_node_get_parent_by_path_return_val;
}

void test_ush_node_mount_already(void)
{       
        struct ush_file_descriptor files[1] = {0};

        ush_node_get_by_path_path = "test";
        ush_node_get_by_path_return_val = &node1;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_ALREADY_MOUNTED, ush_node_mount(&ush, "test", &node1, files, 1));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_NULL(ush.root);
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_ALREADY_MOUNTED, ush_node_mount(&ush, "test", &node2, files, 1));
        TEST_ASSERT_EQUAL(2, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_NULL(ush.root);
}

void test_ush_node_mount_no_parent(void)
{        
        struct ush_file_descriptor files[1] = {0};

        ush_node_get_by_path_path = "test_some";
        ush_node_get_by_path_return_val = NULL;
        ush_node_get_parent_by_path_path = "test_some";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_WITHOUT_PARENT, ush_node_mount(&ush, "test_some", &node1, files, 1));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_NULL(ush.root);

        setUp();
        
        ush_node_get_by_path_path = "/test_some";
        ush_node_get_by_path_return_val = NULL;
        ush_node_get_parent_by_path_path = "/test_some";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_WITHOUT_PARENT, ush_node_mount(&ush, "/test_some", &node1, files, 1));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_NULL(ush.root);
}

void test_ush_node_mount_root(void)
{        
        struct ush_file_descriptor files[1] = {0};

        ush_node_get_by_path_path = "/";
        ush_node_get_by_path_return_val = NULL;
        ush_node_get_parent_by_path_path = "/";
        ush_node_get_parent_by_path_return_val = NULL;
        node1.parent = (struct ush_node_object*)1234;
        node1.next = (struct ush_node_object*)1234;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_mount(&ush, "/", &node1, files, 1));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node1, ush.root);
        TEST_ASSERT_EQUAL(&node1, ush.current_node);
        TEST_ASSERT_NULL(node1.parent);
        TEST_ASSERT_NULL(node1.next);
        TEST_ASSERT_NULL(node1.children);
}

void test_ush_node_mount(void)
{       
        struct ush_file_descriptor files[1] = {0};

        node1.children = &node2;
        node2.next = NULL;
        ush_node_get_by_path_path = "/";
        ush_node_get_by_path_return_val = NULL;
        ush_node_get_parent_by_path_path = "/";
        ush_node_get_parent_by_path_return_val = &node1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_mount(&ush, "/", &node3, files, 1));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_NULL(ush.root);
        TEST_ASSERT_EQUAL(&node3, node1.children);
        TEST_ASSERT_EQUAL(&node2, node3.next);
        TEST_ASSERT_NULL(node2.next);
}

void test_ush_node_unmount_not_found(void)
{
        ush.root = &node2;
        ush_node_get_by_path_path = "test";
        ush_node_get_by_path_return_val = NULL;
        ush_node_get_parent_by_path_path = "test";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_unmount(&ush, "test"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node2, ush.root);

        setUp();

        ush.root = &node2;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node1;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node2, ush.root);

        setUp();

        ush.root = &node2;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node1;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node2, ush.root);

        setUp();

        ush.root = &node1;
        node1.children = NULL;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node2;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = &node1;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node1, ush.root);
        TEST_ASSERT_NULL(node1.children);
}

void test_ush_node_unmount_parents(void)
{
        ush.root = &node1;
        node1.children = (struct ush_node_object*)1234;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node1;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_WITH_CHILDS, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node1, ush.root);

        setUp();

        ush.root = &node1;
        node1.children = &node2;
        node2.next = &node3;
        node3.next = NULL;
        node2.children = (struct ush_node_object*)1234;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node2;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = &node1;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_WITH_CHILDS, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node1, ush.root);
        TEST_ASSERT_EQUAL(&node2, node1.children);
        TEST_ASSERT_EQUAL(&node3, node2.next);
        TEST_ASSERT_NULL(node3.next);
}

void test_ush_node_unmount_root(void)
{
        ush.root = &node1;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node1;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_NULL(ush.root);
}

void test_ush_node_unmount(void)
{
        ush.root = &node1;
        node1.children = &node2;
        node2.next = &node3;
        node3.next = NULL;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node2;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = &node1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node1, ush.root);
        TEST_ASSERT_EQUAL(&node3, node1.children);
        TEST_ASSERT_NULL(node3.next);

        setUp();

        ush.root = &node1;
        node1.children = &node2;
        node2.next = &node3;
        node3.next = NULL;
        ush_node_get_by_path_path = "test2";
        ush_node_get_by_path_return_val = &node3;
        ush_node_get_parent_by_path_path = "test2";
        ush_node_get_parent_by_path_return_val = &node1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_unmount(&ush, "test2"));
        TEST_ASSERT_EQUAL(1, ush_node_get_by_path_call_count);
        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);
        TEST_ASSERT_EQUAL(&node1, ush.root);
        TEST_ASSERT_EQUAL(&node2, node1.children);
        TEST_ASSERT_NULL(node2.next);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_node_mount_already);
        RUN_TEST(test_ush_node_mount_no_parent);
        RUN_TEST(test_ush_node_mount_root);
        RUN_TEST(test_ush_node_mount);
        RUN_TEST(test_ush_node_unmount_not_found);
        RUN_TEST(test_ush_node_unmount_parents);
        RUN_TEST(test_ush_node_unmount_root);
        RUN_TEST(test_ush_node_unmount);

        return UNITY_END();
}
