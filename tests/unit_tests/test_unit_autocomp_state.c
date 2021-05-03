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
int ush_autocomp_check_for_next_call_count;
int ush_autocomp_process_file_index_call_count;
int ush_autocomp_process_file_prepare_call_count;

bool ush_autocomp_check_for_next_return_val;
struct ush_file_descriptor *ush_autocomp_process_file_index_file;
bool ush_autocomp_process_file_prepare_return_val;
struct ush_file_descriptor *ush_autocomp_process_file_prepare_file_in;
struct ush_file_descriptor *ush_autocomp_process_file_prepare_file_out;

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
        ush_autocomp_check_for_next_call_count = 0;
        ush_autocomp_process_file_index_call_count = 0;
        ush_autocomp_process_file_prepare_call_count = 0;
        
        ush_autocomp_check_for_next_return_val = false;
        ush_autocomp_process_file_index_file = NULL;

        ush_autocomp_process_file_prepare_return_val = false;
        ush_autocomp_process_file_prepare_file_in = NULL;
        ush_autocomp_process_file_prepare_file_out = NULL;

        g_assert_call_count = 0;
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

bool ush_autocomp_check_for_next(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_autocomp_check_for_next_call_count++;

        return ush_autocomp_check_for_next_return_val;
}

void ush_autocomp_process_file_index(struct ush_object *self, const struct ush_file_descriptor *file)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_autocomp_process_file_index_file, file);

        ush_autocomp_process_file_index_call_count++;
}

bool ush_autocomp_process_file_prepare(struct ush_object *self, struct ush_file_descriptor const **file)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_autocomp_process_file_prepare_file_in, *file);
        *file = ush_autocomp_process_file_prepare_file_out;

        ush_autocomp_process_file_prepare_call_count++;

        return ush_autocomp_process_file_prepare_return_val;
}

void test_ush_autocomp_state_recall_suffix(void)
{
        setUp();
        ush.autocomp_input = "test";
        ush.autocomp_suffix_len = 3;
        ush_write_pointer_state = USH_STATE_READ_CHAR;
        ush_write_pointer_text = "est";
        ush_autocomp_state_recall_suffix(&ush);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);

        setUp();
        ush.autocomp_input = "test";
        ush.autocomp_suffix_len = 0;
        ush_write_pointer_state = USH_STATE_AUTOCOMP_CANDIDATES_PRINT;
        ush_write_pointer_text = "\r\n";
        ush_autocomp_state_recall_suffix(&ush);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
}

void test_ush_autocomp_state_prepare(void)
{
        setUp();
        strcpy(input_buf, "test abc qwerty   ");
        ush.state = USH_STATE__TOTAL_NUM;
        ush_autocomp_state_prepare(&ush);
        TEST_ASSERT_EQUAL_STRING("", ush.autocomp_input);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_START, ush.state);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);

        setUp();
        strcpy(input_buf, "test abc qwerty");
        ush.state = USH_STATE__TOTAL_NUM;
        ush_autocomp_state_prepare(&ush);
        TEST_ASSERT_EQUAL_STRING("qwerty", ush.autocomp_input);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_START, ush.state);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
}

void test_ush_autocomp_state_candidates_start(void)
{
        ush.autocomp_prev_count = 10;
        ush.autocomp_suffix_len = 20;
        ush_autocomp_state_candidates_start(&ush);
        TEST_ASSERT_EQUAL(0, ush.autocomp_prev_count);
        TEST_ASSERT_EQUAL(0, ush.autocomp_suffix_len);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_COUNT, ush.state);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);        
}

void test_ush_autocomp_state_candidates_finish(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.autocomp_prev_state = state;
                ush.state = USH_STATE__TOTAL_NUM;

                switch (state) {
                case USH_STATE_AUTOCOMP_CANDIDATES_PRINT:
                        ush_autocomp_state_candidates_finish(&ush);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_PROMPT, ush.state);
                        break;
                case USH_STATE_AUTOCOMP_CANDIDATES_COUNT:
                        for (int n = 0; n < 10; n++) {
                                setUp();
                                ush.autocomp_prev_state = state;
                                ush.state = USH_STATE__TOTAL_NUM;
                                ush.autocomp_count = n;

                                switch (n) {
                                case 0:
                                        ush_autocomp_state_candidates_finish(&ush);
                                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                                        TEST_ASSERT_EQUAL(USH_STATE_READ_CHAR, ush.state);
                                        break;
                                case 1:
                                        strcpy(input_buf, "test");
                                        ush.autocomp_input = &input_buf[2];
                                        ush.autocomp_candidate_name = "abcd";
                                        ush_write_pointer_text = "cd";
                                        ush_write_pointer_state = USH_STATE_READ_CHAR;
                                        ush_autocomp_state_candidates_finish(&ush);
                                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                                        TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, ush.state);
                                        TEST_ASSERT_EQUAL(6, ush.in_pos);
                                        break;
                                default:
                                        ush.autocomp_suffix_len = 10;
                                        ush_autocomp_state_candidates_finish(&ush);
                                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                                        TEST_ASSERT_EQUAL(1, ush_autocomp_optimize_continue_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                                        TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, ush.state);
                                        break;
                                }
                        }
                        break;
                case USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE:
                        setUp();
                        ush.autocomp_prev_state = state;
                        ush.state = USH_STATE__TOTAL_NUM;
                        ush_autocomp_state_candidates_finish(&ush);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                        TEST_ASSERT_EQUAL(1, ush_autocomp_optimize_continue_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                        TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, ush.state);

                        setUp();
                        ush.autocomp_prev_state = state;
                        ush.state = USH_STATE__TOTAL_NUM;
                        ush.in_pos = 2;
                        ush.autocomp_prev_count = 1;
                        ush.autocomp_suffix_len = 2;
                        input_buf[0] = '\xFF';
                        input_buf[1] = '\xFF';
                        ush_autocomp_state_candidates_finish(&ush);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                        TEST_ASSERT_EQUAL(1, ush.autocomp_suffix_len);
                        TEST_ASSERT_EQUAL(1, ush.in_pos);
                        TEST_ASSERT_EQUAL('\xFF', input_buf[0]);
                        TEST_ASSERT_EQUAL('\0', input_buf[1]);
                        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_RECALL_SUFFIX, ush.state);

                        setUp();
                        ush.autocomp_prev_state = state;
                        ush.state = USH_STATE__TOTAL_NUM;
                        ush.in_pos = 0;
                        ush.autocomp_prev_count = 1;
                        ush.autocomp_suffix_len = 1;
                        input_buf[0] = '\xFF';
                        input_buf[1] = '\xFF';
                        ush_autocomp_state_candidates_finish(&ush);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                        TEST_ASSERT_EQUAL(0, ush.autocomp_suffix_len);
                        TEST_ASSERT_EQUAL(0, ush.in_pos);
                        TEST_ASSERT_EQUAL('\0', input_buf[0]);
                        TEST_ASSERT_EQUAL('\xFF', input_buf[1]);
                        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_RECALL_SUFFIX, ush.state);
                        break;
                default:
                        ush_autocomp_state_candidates_finish(&ush);
                        TEST_ASSERT_EQUAL(1, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);
                        TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, ush.state);
                        break;
                }
        }
}

void test_ush_autocomp_state_candidates_process(void)
{
        setUp();
        ush.process_node = NULL;
        ush_autocomp_state_candidates_process(&ush);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);

        setUp();
        ush.process_node = (struct ush_node_object*)1234;
        ush_autocomp_check_for_next_return_val = true;
        ush_autocomp_state_candidates_process(&ush);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_prepare_call_count);

        setUp();
        ush.process_node = (struct ush_node_object*)1234;
        ush_autocomp_process_file_prepare_file_in = NULL;
        ush_autocomp_process_file_prepare_file_out = (struct ush_file_descriptor*)5678;
        ush_autocomp_process_file_prepare_return_val = true;
        ush_autocomp_state_candidates_process(&ush);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_process_file_prepare_call_count);

        setUp();
        ush.process_node = (struct ush_node_object*)1234;
        ush_autocomp_process_file_prepare_file_in = NULL;
        ush_autocomp_process_file_prepare_file_out = (struct ush_file_descriptor*)5678;
        ush_autocomp_process_file_index_file = (struct ush_file_descriptor*)5678;
        ush_autocomp_state_candidates_process(&ush);
        TEST_ASSERT_EQUAL(0, g_assert_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_prepare_candidates_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_optimize_continue_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_check_for_finish_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_check_for_next_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_process_file_index_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_process_file_prepare_call_count);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_autocomp_state_recall_suffix);
        RUN_TEST(test_ush_autocomp_state_prepare);
        RUN_TEST(test_ush_autocomp_state_candidates_start);
        RUN_TEST(test_ush_autocomp_state_candidates_process);
        RUN_TEST(test_ush_autocomp_state_candidates_finish);

        return UNITY_END();
}
