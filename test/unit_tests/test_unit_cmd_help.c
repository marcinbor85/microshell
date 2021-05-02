#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_commands.h"

struct ush_object ush;

ush_status_t ush_print_status_status;
int ush_print_status_call_count;

char* ush_print_no_newline_buf;
int ush_print_no_newline_call_count;

char* ush_file_find_by_name_name[4];
int ush_file_find_by_name_call_count;
struct ush_file_descriptor *ush_file_find_by_name_return_val[4];
int ush_file_find_by_name_index;

struct ush_file_descriptor *ush_process_start_file;
int ush_process_start_call_count;

int ush_write_pointer_call_count;
char *ush_write_pointer_text;
ush_state_t ush_write_pointer_state;

extern void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern bool ush_buildin_cmd_help_service(struct ush_object *self, struct ush_file_descriptor const *file);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        ush_process_start_file = NULL;
        ush_process_start_call_count = 0;

        ush_print_no_newline_buf = NULL;
        ush_print_no_newline_call_count = 0;

        memset((uint8_t*)ush_file_find_by_name_name, 0, sizeof(ush_file_find_by_name_name));
        memset((uint8_t*)ush_file_find_by_name_return_val, 0, sizeof(ush_file_find_by_name_return_val));
        ush_file_find_by_name_call_count = 0;
        ush_file_find_by_name_index = 0;

        ush_write_pointer_call_count = 0;
        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;
}

void tearDown(void)
{

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

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_file_find_by_name_name[ush_file_find_by_name_index], name);

        ush_file_find_by_name_call_count++;

        return ush_file_find_by_name_return_val[ush_file_find_by_name_index++];
}

void ush_print_no_newline(struct ush_object *self, char *buf)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_print_no_newline_buf, buf);

        ush_print_no_newline_call_count++;
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_write_pointer_text, text);
        TEST_ASSERT_EQUAL(ush_write_pointer_state, state);

        ush_write_pointer_call_count++;
}

void test_ush_buildin_cmd_help_callback_wrong_arguments(void)
{
        setUp();
        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
        ush_buildin_cmd_help_callback(&ush, NULL, 0, NULL);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_no_newline_call_count);

        setUp();
        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
        ush_buildin_cmd_help_callback(&ush, NULL, 3, NULL);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_no_newline_call_count);

        setUp();
        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
        ush_buildin_cmd_help_callback(&ush, NULL, 4, NULL);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_no_newline_call_count);
}

void test_ush_buildin_cmd_help_callback_selfhelp(void)
{
        struct ush_file_descriptor *file = (struct ush_file_descriptor *)1234;

        ush.process_index = 1;
        ush.process_index_item = 1;
        ush.commands = (struct ush_node_object*)5678;
        ush_process_start_file = (struct ush_file_descriptor *)1234;
        ush_buildin_cmd_help_callback(&ush, file, 1, NULL);
        TEST_ASSERT_EQUAL(0, ush.process_index);
        TEST_ASSERT_EQUAL(0, ush.process_index_item);
        TEST_ASSERT_EQUAL((struct ush_node_object*)5678, ush.process_node);
        TEST_ASSERT_EQUAL(1, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_no_newline_call_count);
}

void test_ush_buildin_cmd_help_callback_file(void)
{
        struct ush_file_descriptor file = {0};
        char *argv[4] = {0};

        setUp();
        argv[1] = "test";
        ush_file_find_by_name_name[0] = "test";
        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_FOUND;
        ush_file_find_by_name_return_val[0] = NULL;
        ush_buildin_cmd_help_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_no_newline_call_count);

        setUp();
        argv[1] = "test1";
        ush_file_find_by_name_name[0] = "test1";
        file.help = NULL;
        ush_print_status_status = USH_STATUS_ERROR_FILE_NO_HELP;
        ush_file_find_by_name_return_val[0] = &file;
        ush_buildin_cmd_help_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_no_newline_call_count);

        setUp();
        argv[1] = "test2";
        ush_file_find_by_name_name[0] = "test2";
        file.help = "help";
        ush_print_status_status = USH_STATUS_ERROR_FILE_NO_HELP;
        ush_file_find_by_name_return_val[0] = &file;
        ush_print_no_newline_buf = "help";
        ush_buildin_cmd_help_callback(&ush, NULL, 2, argv);
        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(1, ush_print_no_newline_call_count);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_help_callback_wrong_arguments);
        RUN_TEST(test_ush_buildin_cmd_help_callback_selfhelp);
        RUN_TEST(test_ush_buildin_cmd_help_callback_file);

        return UNITY_END();
}
