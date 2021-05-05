#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "inc/ush_internal.h"

int g_assert_call_count;

char input_buffer[128];
char output_buffer[128];

struct ush_descriptor ush_desc;
struct ush_object ush;

int ush_write_pointer_call_count;
char ush_read_echo_service_char;

void setUp(void)
{
        ush_desc.input_buffer = input_buffer;
        ush_desc.input_buffer_size = sizeof(input_buffer);
        ush_desc.output_buffer = output_buffer;
        ush_desc.output_buffer_size = sizeof(output_buffer);

        memset((uint8_t*)&ush, 0, sizeof(ush));
        memset((uint8_t*)output_buffer, 0, sizeof(output_buffer));
        memset((uint8_t*)input_buffer, 0, sizeof(input_buffer));

        ush.desc = &ush_desc;

        ush_write_pointer_call_count = 0;
        ush_read_echo_service_char = '\0';
}

void tearDown(void)
{

}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(output_buffer, text);

        switch (ush_read_echo_service_char) {
        case '\r':
        case '\n':
                TEST_ASSERT_EQUAL(USH_STATE_PARSE_PREPARE, state);
                break;
        default:
                TEST_ASSERT_EQUAL(USH_STATE_READ_CHAR, state);
                break;
        }

        ush_write_pointer_call_count++;
}

void test_ush_read_echo_service(void)
{
        for (int i = 0; i < 256; i++) {
                setUp();

                char ch = (char)i;
                ush_read_echo_service_char = ch;
                ush_read_echo_service(&ush, ch);

                switch (ch) {
                case '\r':
                        TEST_ASSERT_EQUAL(ch, output_buffer[0]);
                        TEST_ASSERT_EQUAL('\n', output_buffer[1]);
                        TEST_ASSERT_EQUAL('\0', output_buffer[2]);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        break;
                case '\x08':
                case '\x7F':
                        TEST_ASSERT_EQUAL('\x08', output_buffer[0]);
                        TEST_ASSERT_EQUAL(' ', output_buffer[1]);
                        TEST_ASSERT_EQUAL('\x08', output_buffer[2]);
                        TEST_ASSERT_EQUAL('\0', output_buffer[3]);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        break;
                default:
                        TEST_ASSERT_EQUAL(ch, output_buffer[0]);
                        TEST_ASSERT_EQUAL('\0', output_buffer[1]);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        break;
                }
        }
}

void test_ush_read_char_by_escape_state_0(void)
{
        for (int i = 0; i < 256; i++) {
                setUp();

                char ch = (char)i;

                ush.ansi_escape_state = 0;
                ush.in_pos = 0;
                input_buffer[0] = '\x7F';
                input_buffer[1] = '\x7F';
                TEST_ASSERT_TRUE(ush_read_char_by_escape_state(&ush, ch));
                TEST_ASSERT_EQUAL(ch, input_buffer[0]);
                TEST_ASSERT_EQUAL('\0', input_buffer[1]);
                TEST_ASSERT_EQUAL(1, ush.in_pos);

                ush.ansi_escape_state = 0;
                ush.in_pos = sizeof(input_buffer) - 1;
                input_buffer[sizeof(input_buffer) - 1] = '\x7F';
                input_buffer[0] = '\x7F';
                TEST_ASSERT_TRUE(ush_read_char_by_escape_state(&ush, ch));
                TEST_ASSERT_EQUAL(ch, input_buffer[sizeof(input_buffer) - 1]);
                TEST_ASSERT_EQUAL('\0', input_buffer[0]);
                TEST_ASSERT_EQUAL(0, ush.in_pos);
        }
}

void test_ush_read_char_by_escape_state_1(void)
{
        for (int i = 0; i < 256; i++) {
                setUp();

                char ch = (char)i;
                ush.ansi_escape_state = 1;
                TEST_ASSERT_FALSE(ush_read_char_by_escape_state(&ush, ch));

                switch (ch) {
                case '\x5B':
                case '\x4F':        
                        TEST_ASSERT_EQUAL(2, ush.ansi_escape_state);
                        break;
                default:
                        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
                        break;
                }
        }
}

void test_ush_read_char_by_escape_state_2(void)
{
        for (int i = 0; i < 256; i++) {
                setUp();

                char ch = (char)i;
                ush.ansi_escape_state = 2;
                TEST_ASSERT_FALSE(ush_read_char_by_escape_state(&ush, ch));
                TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
        }
}

void test_ush_read_start(void)
{
        ush.in_pos = 4;
        ush.ansi_escape_state = 1;
        ush.state = USH_STATE__TOTAL_NUM;
        ush.desc->input_buffer[0] = '\x01';
        ush_read_start(&ush);
        TEST_ASSERT_EQUAL(0, ush.in_pos);
        TEST_ASSERT_EQUAL(0, ush.ansi_escape_state);
        TEST_ASSERT_EQUAL(USH_STATE_READ_CHAR, ush.state);
        TEST_ASSERT_EQUAL('\0', ush.desc->input_buffer[0]);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_read_echo_service);
        RUN_TEST(test_ush_read_char_by_escape_state_0);
        RUN_TEST(test_ush_read_char_by_escape_state_1);
        RUN_TEST(test_ush_read_char_by_escape_state_2);
        RUN_TEST(test_ush_read_start);

        return UNITY_END();
}
