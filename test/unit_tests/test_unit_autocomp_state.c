#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"
#include "ush_const.h"
#include "ush.h"

int g_assert_call_count;

char input_buf[256];

struct ush_object ush;
struct ush_descriptor ush_desc;

char *ush_write_pointer_text;
ush_state_t ush_write_pointer_state;
int ush_write_pointer_call_count;

int ush_autocomp_prepare_candidates_call_count;
int ush_autocomp_optimize_continue_call_count;
int ush_autocomp_check_for_finish_call_count;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.input_buffer = input_buf;
        ush_desc.input_buffer_size = sizeof(input_buf);
        ush.desc = &ush_desc;

        ush_write_pointer_call_count = 0;
        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;

        ush_autocomp_prepare_candidates_call_count = 0;
        ush_autocomp_optimize_continue_call_count = 0;
        ush_autocomp_check_for_finish_call_count = 0;
}

void tearDown(void)
{

}


void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_write_pointer_text, text);
        TEST_ASSERT_EQUAL(ush_write_pointer_state, state);

        ush_write_pointer_call_count++;
}

void ush_autocomp_prepare_candidates(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_autocomp_prepare_candidates_call_count++;
}

void ush_autocomp_optimize_continue(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_autocomp_optimize_continue_call_count++;
}

void ush_autocomp_check_for_finish(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_autocomp_check_for_finish_call_count++;
}

void test_ush_autocomp_state_recall_suffix(void)
{
        setUp();
        ush.autocomp_input = "test";
        ush.autocomp_suffix_len = 3;
        ush_write_pointer_state = USH_STATE_READ_CHAR;
        ush_write_pointer_text = "est";
        ush_autocomp_state_recall_suffix(&ush);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);

        setUp();
        ush.autocomp_input = "test";
        ush.autocomp_suffix_len = 0;
        ush_write_pointer_state = USH_STATE_AUTOCOMP_CANDIDATES_PRINT;
        ush_write_pointer_text = "\r\n";
        ush_autocomp_state_recall_suffix(&ush);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
}

void test_ush_autocomp_state_prepare(void)
{
        setUp();
        strcpy(input_buf, "test abc qwerty   ");
        ush.state = USH_STATE__TOTAL_NUM;
        ush_autocomp_state_prepare(&ush);
        TEST_ASSERT_EQUAL_STRING("", ush.autocomp_input);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_START, ush.state);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);

        setUp();
        strcpy(input_buf, "test abc qwerty");
        ush.state = USH_STATE__TOTAL_NUM;
        ush_autocomp_state_prepare(&ush);
        TEST_ASSERT_EQUAL_STRING("qwerty", ush.autocomp_input);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_START, ush.state);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
}

void test_ush_autocomp_state_candidates_start(void)
{
        ush.autocomp_prev_count = 10;
        ush.autocomp_suffix_len = 20;
        ush_autocomp_state_candidates_start(&ush);
        TEST_ASSERT_EQUAL(0, ush.autocomp_prev_count);
        TEST_ASSERT_EQUAL(0, ush.autocomp_suffix_len);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_COUNT, ush.state);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_autocomp_state_recall_suffix);
        RUN_TEST(test_ush_autocomp_state_prepare);
        RUN_TEST(test_ush_autocomp_state_candidates_start);

        return UNITY_END();
}
