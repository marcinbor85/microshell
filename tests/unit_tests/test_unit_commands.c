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

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
}

void tearDown(void)
{

}

void test_ush_commands_add(void)
{
        struct ush_node_object node = {0};
        struct ush_file_descriptor files[4] = {0};

        for (int i = 0; i < 4; i++) {
                setUp();
                node.path = "test";
                ush.commands = (struct ush_node_object*)1234;
                TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_add(&ush, &node, files, i));
                TEST_ASSERT_EQUAL(&files, node.file_list);
                TEST_ASSERT_EQUAL(i, node.file_list_size);
                TEST_ASSERT_NULL(node.path);
                TEST_ASSERT_EQUAL((struct ush_node_object*)1234, node.next);
                TEST_ASSERT_EQUAL(&node, ush.commands);
        }
}

void test_ush_commands_add_multi(void)
{
        struct ush_file_descriptor files = {0};

        struct ush_node_object cmd1 = {0};
        struct ush_node_object cmd2 = {0};
        struct ush_node_object cmd3 = {0};

        ush.commands = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_add(&ush, &cmd1, &files, 1));
        TEST_ASSERT_EQUAL(&cmd1, ush.commands);
        TEST_ASSERT_NULL(cmd1.next);

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_add(&ush, &cmd2, &files, 1));
        TEST_ASSERT_EQUAL(&cmd2, ush.commands);
        TEST_ASSERT_EQUAL(&cmd1, cmd2.next);
        TEST_ASSERT_NULL(cmd1.next);

        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_add(&ush, &cmd3, &files, 1));
        TEST_ASSERT_EQUAL(&cmd3, ush.commands);
        TEST_ASSERT_EQUAL(&cmd2, cmd3.next);
        TEST_ASSERT_EQUAL(&cmd1, cmd2.next);
        TEST_ASSERT_NULL(cmd1.next);
}

void test_ush_commands_remove_not_exist_empty(void)
{
        struct ush_node_object node = {0};

        ush.commands = NULL;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_commands_remove(&ush, &node));
}

void test_ush_commands_remove_not_exist_full(void)
{
        struct ush_node_object node = {0};
        struct ush_node_object commands = {0};

        ush.commands = &commands;
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_commands_remove(&ush, &node));
}


void test_ush_commands_remove_exist_one(void)
{
        struct ush_node_object cmd1 = {0};

        ush.commands = &cmd1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_remove(&ush, &cmd1));
        TEST_ASSERT_NULL(ush.commands);
}

void test_ush_commands_remove_exist_multi_first(void)
{
        struct ush_node_object cmd1 = {0};
        struct ush_node_object cmd2 = {0};
        struct ush_node_object cmd3 = {0};

        cmd1.next = &cmd2;
        cmd2.next = &cmd3;
        cmd3.next = NULL;

        ush.commands = &cmd1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_remove(&ush, &cmd1));
        TEST_ASSERT_EQUAL(&cmd2, ush.commands);
        TEST_ASSERT_EQUAL(&cmd3, cmd2.next);
        TEST_ASSERT_NULL(cmd3.next);
}

void test_ush_commands_remove_exist_multi_mid(void)
{
        struct ush_node_object cmd1 = {0};
        struct ush_node_object cmd2 = {0};
        struct ush_node_object cmd3 = {0};

        cmd1.next = &cmd2;
        cmd2.next = &cmd3;
        cmd3.next = NULL;

        ush.commands = &cmd1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_remove(&ush, &cmd2));
        TEST_ASSERT_EQUAL(&cmd1, ush.commands);
        TEST_ASSERT_EQUAL(&cmd3, cmd1.next);
        TEST_ASSERT_NULL(cmd3.next);
}

void test_ush_commands_remove_exist_multi_last(void)
{
        struct ush_node_object cmd1 = {0};
        struct ush_node_object cmd2 = {0};
        struct ush_node_object cmd3 = {0};

        cmd1.next = &cmd2;
        cmd2.next = &cmd3;
        cmd3.next = NULL;

        ush.commands = &cmd1;
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_remove(&ush, &cmd3));
        TEST_ASSERT_EQUAL(&cmd1, ush.commands);
        TEST_ASSERT_EQUAL(&cmd2, cmd1.next);
        TEST_ASSERT_NULL(cmd2.next);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_commands_add);
        RUN_TEST(test_ush_commands_add_multi);
        RUN_TEST(test_ush_commands_remove_not_exist_empty);
        RUN_TEST(test_ush_commands_remove_not_exist_full);
        RUN_TEST(test_ush_commands_remove_exist_one);
        RUN_TEST(test_ush_commands_remove_exist_multi_first);
        RUN_TEST(test_ush_commands_remove_exist_multi_mid);
        RUN_TEST(test_ush_commands_remove_exist_multi_last);

        return UNITY_END();
}
