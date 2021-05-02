#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"
#include "ush_const.h"
#include "ush.h"

char input_buf[4];

struct ush_object ush;
struct ush_descriptor ush_desc;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.input_buffer = input_buf;
        ush_desc.input_buffer_size = sizeof(input_buf);
        ush.desc = &ush_desc;
}

void tearDown(void)
{

}

void test_ush_autocomp_start(void)
{
        ush.state = USH_STATE__TOTAL_NUM;
        ush_autocomp_start(&ush);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_PREPARE, ush.state);
}

void test_ush_autocomp_prepare_candidates(void)
{
        ush.state = USH_STATE__TOTAL_NUM;
        ush.commands = (struct ush_node_object*)1234;
        ush.autocomp_count = 10;
        ush.process_stage = 10;
        ush.process_index = 10;
        ush.process_index_item = 10;
        ush_autocomp_prepare_candidates(&ush);
        TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, ush.state);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
        TEST_ASSERT_EQUAL(0, ush.autocomp_count);
        TEST_ASSERT_EQUAL(0, ush.process_stage);
        TEST_ASSERT_EQUAL(0, ush.process_index);
        TEST_ASSERT_EQUAL(0, ush.process_index_item);
}

void test_ush_autocomp_optimize_continue(void)
{
        setUp();
        ush.state = USH_STATE__TOTAL_NUM;
        ush.autocomp_count = 10;
        ush.autocomp_prev_count = 1;
        ush.autocomp_input = "test";
        ush.autocomp_candidate_name = "test";
        ush_autocomp_optimize_continue(&ush);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_RECALL_SUFFIX, ush.state);
        TEST_ASSERT_EQUAL(10, ush.autocomp_count);
        TEST_ASSERT_EQUAL(10, ush.autocomp_prev_count);

        setUp();
        ush.state = USH_STATE__TOTAL_NUM;
        ush.commands = (struct ush_node_object*)1234;
        ush.autocomp_count = 10;
        ush.autocomp_prev_count = 1;
        ush.autocomp_input = "tes";
        ush.autocomp_candidate_name = "test";
        ush.in_pos = 0;
        ush.autocomp_suffix_len = 5;
        input_buf[0] = '\0';
        ush_autocomp_optimize_continue(&ush);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE, ush.state);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
        TEST_ASSERT_EQUAL(0, ush.autocomp_count);
        TEST_ASSERT_EQUAL(0, ush.process_stage);
        TEST_ASSERT_EQUAL(0, ush.process_index);
        TEST_ASSERT_EQUAL(0, ush.process_index_item);
        TEST_ASSERT_EQUAL(1, ush.in_pos);
        TEST_ASSERT_EQUAL_STRING("t", input_buf);
        TEST_ASSERT_EQUAL(6, ush.autocomp_suffix_len);

        setUp();
        ush.state = USH_STATE__TOTAL_NUM;
        ush.commands = (struct ush_node_object*)1234;
        ush.autocomp_count = 10;
        ush.autocomp_prev_count = 1;
        ush.autocomp_input = "tes";
        ush.autocomp_candidate_name = "test";
        ush.in_pos = 3;
        ush.autocomp_suffix_len = 2;
        input_buf[0] = '\xFF';
        input_buf[1] = '\xFF';
        input_buf[2] = '\xFF';
        input_buf[3] = '\xFF';
        ush_autocomp_optimize_continue(&ush);
        TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_OPTIMISE, ush.state);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
        TEST_ASSERT_EQUAL(0, ush.autocomp_count);
        TEST_ASSERT_EQUAL(0, ush.process_stage);
        TEST_ASSERT_EQUAL(0, ush.process_index);
        TEST_ASSERT_EQUAL(0, ush.process_index_item);
        TEST_ASSERT_EQUAL(0, ush.in_pos);
        TEST_ASSERT_EQUAL('t', input_buf[3]);
        TEST_ASSERT_EQUAL('\0', input_buf[0]);
        TEST_ASSERT_EQUAL('\xFF', input_buf[1]);
        TEST_ASSERT_EQUAL('\xFF', input_buf[2]);
        TEST_ASSERT_EQUAL(3, ush.autocomp_suffix_len);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_autocomp_start);
        RUN_TEST(test_ush_autocomp_prepare_candidates);
        RUN_TEST(test_ush_autocomp_optimize_continue);

        return UNITY_END();
}
