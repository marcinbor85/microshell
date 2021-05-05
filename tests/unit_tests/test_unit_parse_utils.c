#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "inc/ush_internal.h"

int g_assert_call_count;

char input_buffer[128];
struct ush_object ush;
struct ush_descriptor ush_desc;

void setUp(void)
{
        ush_desc.input_buffer = input_buffer;
        ush_desc.input_buffer_size = sizeof(input_buffer);
        ush_desc.path_max_length = 256;

        memset((uint8_t*)&ush, 0, sizeof(ush));
        memset(input_buffer, 0, sizeof(input_buffer));
        ush.desc = &ush_desc;

        g_assert_call_count = 0;
}

void tearDown(void)
{

}

void test_ush_parse_start(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.in_pos = 10;
                ush.out_pos = 5;
                ush.args_count = 2;
                ush.escape_flag = true;
                ush.state = state;

                ush_parse_start(&ush);
                TEST_ASSERT_EQUAL(USH_STATE_PARSE_SEARCH_ARG, ush.state);
        }
}

void test_ush_parse_get_args_null(void)
{
        for (int i = 0; i < 10; i++) {
                setUp();
                ush.args_count = i;
                TEST_ASSERT_EQUAL(i, ush_parse_get_args(&ush, NULL));
        }
}

void test_ush_parse_get_args_data(void)
{
        char *argv[4];

        setUp();
        memset(argv, 0, sizeof(argv));
        memcpy(input_buffer, "test\0", 5);
        ush.args_count = 1;
        TEST_ASSERT_EQUAL(1, ush_parse_get_args(&ush, argv));
        TEST_ASSERT_EQUAL_STRING("test", argv[0]);
        TEST_ASSERT_NULL(argv[1]);
        TEST_ASSERT_NULL(argv[2]);
        TEST_ASSERT_NULL(argv[3]);

        setUp();
        memset(argv, 0, sizeof(argv));
        memcpy(input_buffer, "test\0abc\0", 9);
        ush.args_count = 2;
        TEST_ASSERT_EQUAL(2, ush_parse_get_args(&ush, argv));
        TEST_ASSERT_EQUAL_STRING("test", argv[0]);
        TEST_ASSERT_EQUAL_STRING("abc", argv[1]);
        TEST_ASSERT_NULL(argv[2]);
        TEST_ASSERT_NULL(argv[3]);

        setUp();
        memset(argv, 0, sizeof(argv));
        memcpy(input_buffer, "test\0abc\0qwerty\0", 16);
        ush.args_count = 3;
        TEST_ASSERT_EQUAL(3, ush_parse_get_args(&ush, argv));
        TEST_ASSERT_EQUAL_STRING("test", argv[0]);
        TEST_ASSERT_EQUAL_STRING("abc", argv[1]);
        TEST_ASSERT_EQUAL_STRING("qwerty", argv[2]);
        TEST_ASSERT_NULL(argv[3]);

        setUp();
        memset(argv, 0, sizeof(argv));
        memcpy(input_buffer, "test\0abc\0qwerty\0xyz\0", 20);
        ush.args_count = 4;
        TEST_ASSERT_EQUAL(4, ush_parse_get_args(&ush, argv));
        TEST_ASSERT_EQUAL_STRING("test", argv[0]);
        TEST_ASSERT_EQUAL_STRING("abc", argv[1]);
        TEST_ASSERT_EQUAL_STRING("qwerty", argv[2]);
        TEST_ASSERT_EQUAL_STRING("xyz", argv[3]);

        setUp();
        memset(argv, 0, sizeof(argv));
        memcpy(input_buffer, "test abc\0qwerty xyz\0", 20);
        ush.args_count = 2;
        TEST_ASSERT_EQUAL(2, ush_parse_get_args(&ush, argv));
        TEST_ASSERT_EQUAL_STRING("test abc", argv[0]);
        TEST_ASSERT_EQUAL_STRING("qwerty xyz", argv[1]);
        TEST_ASSERT_NULL(argv[2]);
        TEST_ASSERT_NULL(argv[3]);
}

void test_ush_parse_char_standard_search(void)
{
        for (int i = 0; i < 256; i++) {
                char ch = (char)i;
                char in_char = (char)(~ch);

                setUp();
                ush.args_count = 1;
                ush.out_pos = 1;
                ush.state = USH_STATE_PARSE_SEARCH_ARG;
                input_buffer[1] = in_char;
                ush_parse_char_standard(&ush, ch);

                switch (ch) {
                case ' ':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_SEARCH_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                case '\"':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_QUOTE_ARG, ush.state);
                        TEST_ASSERT_EQUAL(2, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                case '\\':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(2, ush.args_count);
                        TEST_ASSERT_TRUE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                default:
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(2, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(2, ush.out_pos);
                        TEST_ASSERT_EQUAL(ch, input_buffer[1]);
                        break;
                }
        }
}

void test_ush_parse_char_standard_quote(void)
{
        for (int i = 0; i < 256; i++) {
                char ch = (char)i;
                char in_char = (char)(~ch);

                setUp();
                ush.args_count = 1;
                ush.out_pos = 1;
                ush.state = USH_STATE_PARSE_QUOTE_ARG;
                ush.escape_flag = false;
                input_buffer[1] = in_char;
                ush_parse_char_standard(&ush, ch);

                switch (ch) {
                case '\"':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                case '\\':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_QUOTE_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_TRUE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                default:
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_QUOTE_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(2, ush.out_pos);
                        TEST_ASSERT_EQUAL(ch, input_buffer[1]);
                        break;
                }

                setUp();
                ush.args_count = 1;
                ush.out_pos = 1;
                ush.state = USH_STATE_PARSE_QUOTE_ARG;
                ush.escape_flag = true;
                input_buffer[1] = in_char;
                ush_parse_char_standard(&ush, ch);

                switch (ch) {
                case 'x':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_QUOTE_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(3, ush.out_pos);
                        TEST_ASSERT_EQUAL('\\', input_buffer[1]);
                        TEST_ASSERT_EQUAL(ch, input_buffer[2]);
                        break;
                default:
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_QUOTE_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(2, ush.out_pos);
                        TEST_ASSERT_EQUAL(ch, input_buffer[1]);
                        break;
                }
        }
}

void test_ush_parse_char_standard_standard(void)
{
        for (int i = 0; i < 256; i++) {
                char ch = (char)i;
                char in_char = (char)(~ch);

                setUp();
                ush.args_count = 1;
                ush.out_pos = 1;
                ush.state = USH_STATE_PARSE_STANDARD_ARG;
                ush.escape_flag = false;
                input_buffer[1] = in_char;
                ush_parse_char_standard(&ush, ch);

                switch (ch) {
                case ' ':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_SEARCH_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(2, ush.out_pos);
                        TEST_ASSERT_EQUAL('\0', input_buffer[1]);
                        break;
                case '\"':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_QUOTE_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                case '\\':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_TRUE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(1, ush.out_pos);
                        TEST_ASSERT_EQUAL(in_char, input_buffer[1]);
                        break;
                default:
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(2, ush.out_pos);
                        TEST_ASSERT_EQUAL(ch, input_buffer[1]);
                        break;
                }

                setUp();
                ush.args_count = 1;
                ush.out_pos = 1;
                ush.state = USH_STATE_PARSE_STANDARD_ARG;
                ush.escape_flag = true;
                input_buffer[1] = in_char;
                ush_parse_char_standard(&ush, ch);

                switch (ch) {
                case 'x':
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(3, ush.out_pos);
                        TEST_ASSERT_EQUAL('\\', input_buffer[1]);
                        TEST_ASSERT_EQUAL(ch, input_buffer[2]);
                        break;
                default:
                        TEST_ASSERT_EQUAL(USH_STATE_PARSE_STANDARD_ARG, ush.state);
                        TEST_ASSERT_EQUAL(1, ush.args_count);
                        TEST_ASSERT_FALSE(ush.escape_flag);
                        TEST_ASSERT_EQUAL(2, ush.out_pos);
                        TEST_ASSERT_EQUAL(ch, input_buffer[1]);
                        break;
                }                
        }
}

void test_ush_parse_char_standard_states(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.state = state;
                ush_parse_char_standard(&ush, 0);

                switch (state) {
                case USH_STATE_PARSE_SEARCH_ARG:
                case USH_STATE_PARSE_QUOTE_ARG:
                case USH_STATE_PARSE_STANDARD_ARG:
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        break;
                default:
                        TEST_ASSERT_EQUAL(1, g_assert_call_count);
                        break;
                }                
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_parse_start);
        RUN_TEST(test_ush_parse_get_args_null);
        RUN_TEST(test_ush_parse_get_args_data);
        RUN_TEST(test_ush_parse_char_standard_search);
        RUN_TEST(test_ush_parse_char_standard_quote);
        RUN_TEST(test_ush_parse_char_standard_standard);
        RUN_TEST(test_ush_parse_char_standard_states);

        return UNITY_END();
}
