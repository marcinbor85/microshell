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

static struct ush_node_object node = {0};

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{
        test_func_deinit();
}

void test_mount(void)
{
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_set_current_dir(&g_ush, "/abc"));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_mount(&g_ush, "/abc", &node, NULL, 0));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/abc"));
}

void test_mount_error(void)
{
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_ALREADY_MOUNTED, ush_node_mount(&g_ush, "/dir", &node, NULL, 0));
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_WITHOUT_PARENT, ush_node_mount(&g_ush, "/test/abc", &node, NULL, 0));
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_unmount(&g_ush, "/test"));
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_WITH_CHILDS, ush_node_unmount(&g_ush, "/dir"));
}

void test_mount_unmount(void)
{
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/data"));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_unmount(&g_ush, "/data"));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_node_set_current_dir(&g_ush, "/"));
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_node_set_current_dir(&g_ush, "/data"));
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_mount);
        RUN_TEST(test_mount_error);
        RUN_TEST(test_mount_unmount);

        return UNITY_END();
}
