#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ush.h"
#include "test_func.h"

static struct ush_node_object node = {0};

void cmd2_exec_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)file;

        static char buf[128];

        strcpy(buf, "test cmd2\r\n");
        for (int i = 1; i < argc; i++) {
                strcat(buf, argv[i]);
                strcat(buf, "\r\n");
        }
        
        ush_print_no_newline(self, buf);
}

static struct ush_file_descriptor files[] = {
        {
                .name = "cmd1",
                .description = "additional cmd1",
        },
        {
                .name = "cmd2",
                .description = "additional cmd2",
                .exec = cmd2_exec_callback,
        }
};

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{
        test_func_deinit();
}

void test_commands_add(void)
{
        TEST_FUNC_ASK("cmd1",
                "error: syntax error\r\n"
                "[test /]$ "
        );
        TEST_FUNC_ASK("cmd2",
                "error: syntax error\r\n"
                "[test /]$ "
        );
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_add(&g_ush, &node, files, sizeof(files) / sizeof(files[0])));
        TEST_FUNC_ASK("cmd1",
                "error: file not executable\r\n"
                "[test /]$ "
        );
        TEST_FUNC_ASK("help cmd1",
                "error: no help available\r\n"
                "[test /]$ "
        );
        TEST_FUNC_ASK("cmd2 a b c xyz",
                "test cmd2\r\n"
                "a\r\n"
                "b\r\n"
                "c\r\n"
                "xyz\r\n"
                "[test /]$ "
        );
}

void test_commands_error(void)
{
        TEST_ASSERT_EQUAL(USH_STATUS_ERROR_NODE_NOT_FOUND, ush_commands_remove(&g_ush, &node));
}

void test_commands_remove(void)
{
        TEST_FUNC_ASK("pwd",
                "/\r\n"
                "[test /]$ "
        );
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_add(&g_ush, &node, files, sizeof(files) / sizeof(files[0])));
        TEST_ASSERT_EQUAL(USH_STATUS_OK, ush_commands_remove(&g_ush, &g_ush.buildin_commands));
        TEST_FUNC_ASK("pwd",
                "error: syntax error\r\n"
                "[test /]$ "
        );
        TEST_FUNC_ASK("cmd2 test",
                "test cmd2\r\n"
                "test\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_commands_add);
        RUN_TEST(test_commands_error);
        RUN_TEST(test_commands_remove);

        return UNITY_END();
}
