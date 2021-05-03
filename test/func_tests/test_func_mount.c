#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ush.h"
#include "test_func.h"

struct ush_node_object node = {0};

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
