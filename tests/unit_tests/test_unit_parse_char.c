#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"

int g_assert_call_count;

char input_buffer[128];
struct ush_object ush;
struct ush_descriptor ush_desc;

int ush_parse_finish_call_count;
int ush_parse_char_standard_call_count;
char ush_parse_char_standard_char;

void setUp(void)
{
        ush_desc.input_buffer = input_buffer;
        ush_desc.input_buffer_size = sizeof(input_buffer);
        
        memset((uint8_t*)&ush, 0, sizeof(ush));
        memset(input_buffer, 0, sizeof(input_buffer));
        ush.desc = &ush_desc;

        ush_parse_finish_call_count = 0;
        ush_parse_char_standard_call_count = 0;
        ush_parse_char_standard_char = '\0';
}

void tearDown(void)
{

}

void ush_parse_finish(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_parse_finish_call_count++;
}

void ush_parse_char_standard(struct ush_object *self, char ch)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_parse_char_standard_char, ch);

        ush_parse_char_standard_call_count++;
}

void test_ush_parse_char(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.in_pos = 0;
                ush.out_pos = 0;
                input_buffer[0] = '\r';
                ush.state = state;
                ush_parse_char(&ush);
                TEST_ASSERT_EQUAL(1, ush_parse_finish_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_char_standard_call_count);
                TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
                TEST_ASSERT_EQUAL('\0', input_buffer[0]);
                TEST_ASSERT_EQUAL(1, ush.in_pos);
                TEST_ASSERT_EQUAL(1, ush.out_pos);

                setUp();
                ush.in_pos = 0;
                ush.out_pos = 0;
                input_buffer[0] = '\n';
                ush.state = state;
                ush_parse_char(&ush);
                TEST_ASSERT_EQUAL(1, ush_parse_finish_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_char_standard_call_count);
                TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
                TEST_ASSERT_EQUAL('\0', input_buffer[0]);
                TEST_ASSERT_EQUAL(1, ush.in_pos);
                TEST_ASSERT_EQUAL(1, ush.out_pos);

                setUp();
                ush.in_pos = 1;
                ush.out_pos = 1;
                input_buffer[0] = 'T';
                input_buffer[1] = '\r';
                ush.state = state;
                ush_parse_char(&ush);
                TEST_ASSERT_EQUAL(1, ush_parse_finish_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_char_standard_call_count);
                TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
                TEST_ASSERT_EQUAL('T', input_buffer[0]);
                TEST_ASSERT_EQUAL('\0', input_buffer[1]);
                TEST_ASSERT_EQUAL(2, ush.in_pos);
                TEST_ASSERT_EQUAL(2, ush.out_pos);

                setUp();
                ush.in_pos = 1;
                ush.out_pos = 0;
                input_buffer[0] = 'T';
                input_buffer[1] = '\r';
                ush.state = state;
                ush_parse_char(&ush);
                TEST_ASSERT_EQUAL(1, ush_parse_finish_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_char_standard_call_count);
                TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, ush.state);
                TEST_ASSERT_EQUAL('\0', input_buffer[0]);
                TEST_ASSERT_EQUAL('\r', input_buffer[1]);
                TEST_ASSERT_EQUAL(2, ush.in_pos);
                TEST_ASSERT_EQUAL(1, ush.out_pos);

                for (int n = 0; n < 256; n++) {
                        char ch = (char)n;

                        if (ch == '\r' || ch == '\n')
                                continue;

                        setUp();
                        ush.in_pos = 0;
                        ush.out_pos = 0;
                        input_buffer[0] = ch;
                        ush_parse_char_standard_char = ch;
                        ush.state = state;
                        ush_parse_char(&ush);
                        TEST_ASSERT_EQUAL(0, ush_parse_finish_call_count);
                        TEST_ASSERT_EQUAL(1, ush_parse_char_standard_call_count);
                        TEST_ASSERT_EQUAL(state, ush.state);
                }
        }   
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_parse_char);

        return UNITY_END();
}
