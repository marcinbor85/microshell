#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "inc/ush_internal.h"

int g_assert_call_count;

char input_buffer[128];
struct ush_io_interface ush_io_iface;
struct ush_descriptor ush_desc;
struct ush_object ush;

int read_callback_call_count;
int read_callback_return_val;
char read_callback_read_char;

int ush_autocomp_start_call_count;
int ush_write_pointer_call_count;
int ush_read_echo_service_call_count;

int ush_read_char_by_escape_state_call_count;
bool ush_read_char_by_escape_state_return_val;

int read_callback(struct ush_object *self, char *ch);

void setUp(void)
{
        ush_io_iface.read = read_callback;
        ush_desc.io = &ush_io_iface;
        ush_desc.input_buffer = input_buffer;
        ush_desc.input_buffer_size = sizeof(input_buffer);

        memset((uint8_t*)&ush, 0, sizeof(ush));
        memset((uint8_t*)input_buffer, 0, sizeof(input_buffer));

        ush.desc = &ush_desc;

        read_callback_call_count = 0;
        read_callback_return_val = 0;

        ush_autocomp_start_call_count = 0;
        ush_write_pointer_call_count = 0;
        ush_read_char_by_escape_state_call_count = 0;
        ush_read_echo_service_call_count = 0;

        ush_read_char_by_escape_state_return_val = false;

        read_callback_read_char = '\0';
}

void tearDown(void)
{

}

bool ush_read_char_by_escape_state(struct ush_object *self, char ch)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(read_callback_read_char, ch);

        ush_read_char_by_escape_state_call_count++;

        return ush_read_char_by_escape_state_return_val;
}

void ush_read_echo_service(struct ush_object *self, char ch)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(read_callback_read_char, ch);

        ush_read_echo_service_call_count++;
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_NOT_NULL(text);

        TEST_ASSERT_EQUAL_STRING("^C", text);
        TEST_ASSERT_EQUAL(USH_STATE_RESET, state);

        ush_write_pointer_call_count++;
}

void ush_autocomp_start(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_autocomp_start_call_count++;
}

int read_callback(struct ush_object *self, char *ch)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_NOT_NULL(ch);

        *ch = read_callback_read_char;
        read_callback_call_count++;

        return read_callback_return_val;
}

void test_ush_read_char_no_data(void)
{
        for (int i = 0; i < 256; i++) {
                setUp();
                read_callback_read_char = (char)i;
                read_callback_return_val = 0;
                TEST_ASSERT_FALSE(ush_read_char(&ush));
                TEST_ASSERT_EQUAL(1, read_callback_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
        }
}

void prepare_char_buffer_full(char ch)
{
        setUp();

        ush.in_pos = 1;
        input_buffer[0] = 0x7F;
        read_callback_read_char = ch;
        read_callback_return_val = 1;
}

void test_ush_read_char_buffer_full(void)
{
        for (int i = 0; i < 256; i++) {
                char ch = (char)i;

                switch (ch) {
                case '\x03':
                        prepare_char_buffer_full(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                case '\x08':
                case '\x7F':
                        prepare_char_buffer_full(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_echo_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush.in_pos);
                        TEST_ASSERT_EQUAL('\0', input_buffer[0]);
                        break;
                case '\x09':
                        prepare_char_buffer_full(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(1, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                case '\x1B':
                        prepare_char_buffer_full(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(1, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                default:
                        prepare_char_buffer_full(ch);
                        ush_read_char_by_escape_state_return_val = false;
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);

                        prepare_char_buffer_full(ch);
                        ush_read_char_by_escape_state_return_val = true;
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_echo_service_call_count);
                        break;
                }
        }
}

void prepare_char_buffer_empty(char ch)
{
        setUp();

        ush.in_pos = 0;
        read_callback_read_char = ch;
        read_callback_return_val = 1;
}

void test_ush_read_char_buffer_empty(void)
{
        for (int i = 0; i < 256; i++) {
                char ch = (char)i;

                switch (ch) {
                case '\x03':
                        prepare_char_buffer_empty(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                case '\x08':
                case '\x7F':
                        prepare_char_buffer_empty(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                case '\x09':
                        prepare_char_buffer_empty(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(1, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                case '\x1B':
                        prepare_char_buffer_empty(ch);
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(1, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);
                        break;
                default:
                        prepare_char_buffer_empty(ch);
                        ush_read_char_by_escape_state_return_val = false;
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_echo_service_call_count);

                        prepare_char_buffer_empty(ch);
                        ush_read_char_by_escape_state_return_val = true;
                        TEST_ASSERT_TRUE(ush_read_char(&ush));
                        TEST_ASSERT_EQUAL(1, read_callback_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_char_by_escape_state_call_count);
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_echo_service_call_count);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_read_char_no_data);
        RUN_TEST(test_ush_read_char_buffer_full);
        RUN_TEST(test_ush_read_char_buffer_empty);

        return UNITY_END();
}
