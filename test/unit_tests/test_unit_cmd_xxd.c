#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_commands.h"
#include "ush_shell.h"

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

char* ush_file_find_by_name_name[4];
int ush_file_find_by_name_call_count;
struct ush_file_descriptor *ush_file_find_by_name_return_val[4];
int ush_file_find_by_name_index;

struct ush_file_descriptor *get_data_callback_file;
uint8_t *get_data_callback_data_in;
uint8_t *get_data_callback_data_out;
int get_data_callback_call_count;
size_t get_data_callback_return_val;

extern void ush_buildin_cmd_xxd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[]);
extern bool ush_buildin_cmd_xxd_service(struct ush_object *self, struct ush_file_descriptor const *file);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.output_buffer = output_buf;

        ush.desc = &ush_desc;

        ush_print_status_status = USH_STATUS__TOTAL_NUM;
        ush_print_status_call_count = 0;

        ush_process_start_file = NULL;
        ush_process_start_call_count = 0;

        ush_write_pointer_call_count = 0;
        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;

        memset((uint8_t*)ush_file_find_by_name_name, 0, sizeof(ush_file_find_by_name_name));
        memset((uint8_t*)ush_file_find_by_name_return_val, 0, sizeof(ush_file_find_by_name_return_val));
        ush_file_find_by_name_call_count = 0;
        ush_file_find_by_name_index = 0;

        get_data_callback_file = NULL;
        get_data_callback_data_in = NULL;
        get_data_callback_data_out = NULL;
        get_data_callback_call_count = 0;
        get_data_callback_return_val = 0;

        g_assert_call_count = 0;
}

void tearDown(void)
{

}

struct ush_file_descriptor const* ush_file_find_by_name(struct ush_object *self, const char *name)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_file_find_by_name_name[ush_file_find_by_name_index], name);

        ush_file_find_by_name_call_count++;

        return ush_file_find_by_name_return_val[ush_file_find_by_name_index++];
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

size_t get_data_callback(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(get_data_callback_file, file);
        TEST_ASSERT_EQUAL(get_data_callback_data_in, *data);

        *data = get_data_callback_data_out;
        get_data_callback_call_count++;

        return get_data_callback_return_val;
}

void test_ush_buildin_cmd_xxd_callback(void)
{
        struct ush_file_descriptor file = {0};
        char *argv[4] = {0};

        for (int i = 0; i < 10; i++) {
                setUp();

                switch (i) {
                case 2:
                        setUp();
                        argv[1] = "test";
                        ush_file_find_by_name_name[0] = "test";
                        ush_file_find_by_name_return_val[0] = NULL;
                        ush_print_status_status = USH_STATUS_ERROR_FILE_NOT_FOUND;
                        ush_buildin_cmd_xxd_callback(&ush, NULL, i, argv);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);

                        setUp();
                        argv[1] = "test2";
                        file.get_data = NULL;
                        ush_file_find_by_name_name[0] = "test2";
                        ush_file_find_by_name_return_val[0] = &file;
                        ush_print_status_status = USH_STATUS_ERROR_FILE_NO_DATA;
                        ush_buildin_cmd_xxd_callback(&ush, NULL, i, argv);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);

                        setUp();
                        argv[1] = "test3";
                        file.get_data = get_data_callback;
                        ush_file_find_by_name_name[0] = "test3";
                        ush_file_find_by_name_return_val[0] = &file;
                        ush.process_data = (uint8_t*)1234;
                        get_data_callback_file = &file;
                        get_data_callback_data_in = (uint8_t*)1234;
                        get_data_callback_data_out = (uint8_t*)5678;
                        get_data_callback_return_val = 4321;
                        ush_process_start_file = (struct ush_file_descriptor*)3456;
                        ush_buildin_cmd_xxd_callback(&ush, (struct ush_file_descriptor*)3456, i, argv);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(1, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(1, get_data_callback_call_count);
                        TEST_ASSERT_EQUAL((uint8_t*)5678, ush.process_data);
                        TEST_ASSERT_EQUAL(4321, ush.process_data_size);
                        break;
                default:
                        ush_print_status_status = USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS;
                        ush_buildin_cmd_xxd_callback(&ush, NULL, i, argv);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        break;
                }
        }
}

void test_ush_buildin_cmd_xxd_service_start(void)
{
        struct ush_file_descriptor file = {0};

        ush.state = USH_STATE_PROCESS_START;
        ush.process_index = 10;
        ush.process_index_item = 20;
        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
        TEST_ASSERT_EQUAL(0, ush.process_index);
        TEST_ASSERT_EQUAL(0, ush.process_index_item);
        TEST_ASSERT_EQUAL(USH_STATE_PROCESS_SERVICE, ush.state);
}

void test_ush_buildin_cmd_xxd_service_service(void)
{
        struct ush_file_descriptor file = {0};

        for (int i = 0; i < 10; i++) {
                setUp();
                ush.state = USH_STATE_PROCESS_SERVICE;
                ush.process_index = i;

                switch (i) {
                case 0:
                        setUp();
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush.process_index_item = 0x11223344;
                        ush_write_pointer_text = "11223344: ";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING(output_buf, "11223344: ");
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);

                        setUp();
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush.process_index_item = 0x00AAFF11;
                        ush_write_pointer_text = "00AAFF11: ";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING(output_buf, "00AAFF11: ");
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        break;
                case 1:
                        setUp();
                        ush.process_data = (uint8_t*)"\x00\x11\x22\x33\x44\x55\x66\x77";
                        ush.process_data_size = 8;
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush_write_pointer_text = "00 11 22 33 44 55 66 77  ";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("00 11 22 33 44 55 66 77  ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);

                        setUp();
                        ush.process_data = (uint8_t*)"\xFF\xAA\x11";
                        ush.process_data_size = 3;
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush_write_pointer_text = "FF AA 11                 ";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("FF AA 11                 ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        break;
                case 2:
                        setUp();
                        ush.process_data = (uint8_t*)"abcd";
                        ush.process_data_size = 4;
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush_write_pointer_text = "abcd    ";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING("abcd    ", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);

                        setUp();
                        ush.process_data = (uint8_t*)"\xFF cd\x00\x13 1";
                        ush.process_data_size = 8;
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush_write_pointer_text = ". cd.. 1";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL_STRING(". cd.. 1", output_buf);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        break;
                case 3:
                        setUp();
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush.process_index_item = 10;
                        ush.process_data_size = 10;
                        ush_write_pointer_text = "\r\n";
                        ush_write_pointer_state = USH_STATE_PROCESS_FINISH;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        TEST_ASSERT_EQUAL(i, ush.process_index);

                        setUp();
                        ush.state = USH_STATE_PROCESS_SERVICE;
                        ush.process_index = i;
                        ush.process_index_item = 9;
                        ush.process_data_size = 10;
                        ush_write_pointer_text = "\r\n";
                        ush_write_pointer_state = USH_STATE_PROCESS_SERVICE;
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        break;
                default:
                        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL(1, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        TEST_ASSERT_EQUAL(0, get_data_callback_call_count);
                        break;
                }
        }
}

void test_ush_buildin_cmd_xxd_service_finish(void)
{
        struct ush_file_descriptor file = {0};

        ush.state = USH_STATE_PROCESS_FINISH;
        ush.process_index = 10;
        ush.process_index_item = 20;
        TEST_ASSERT_TRUE(ush_buildin_cmd_xxd_service(&ush, &file));
        TEST_ASSERT_EQUAL(10, ush.process_index);
        TEST_ASSERT_EQUAL(20, ush.process_index_item);
        TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
}

void test_ush_buildin_cmd_xxd_service_states(void)
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
                        TEST_ASSERT_FALSE(ush_buildin_cmd_xxd_service(&ush, &file));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_print_status_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_file_find_by_name_call_count);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_buildin_cmd_xxd_callback);
        RUN_TEST(test_ush_buildin_cmd_xxd_service_start);
        RUN_TEST(test_ush_buildin_cmd_xxd_service_service);
        RUN_TEST(test_ush_buildin_cmd_xxd_service_finish);
        RUN_TEST(test_ush_buildin_cmd_xxd_service_states);

        return UNITY_END();
}
