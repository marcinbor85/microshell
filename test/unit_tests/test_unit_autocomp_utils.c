#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"
#include "ush_const.h"
#include "ush.h"

int g_assert_call_count;

char input_buf[4];

struct ush_object ush;
struct ush_descriptor ush_desc;

char *ush_node_get_absolute_path_in_path;
char *ush_node_get_absolute_path_out_path;
int ush_node_get_absolute_path_call_count;

char *ush_node_get_parent_by_path_path;
int ush_node_get_parent_by_path_call_count;
struct ush_node_object *ush_node_get_parent_by_path_return_val;

int ush_write_pointer_call_count;
char *ush_write_pointer_text;
ush_state_t ush_write_pointer_state;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.input_buffer = input_buf;
        ush_desc.input_buffer_size = sizeof(input_buf);
        ush.desc = &ush_desc;

        ush_node_get_absolute_path_in_path = NULL;
        ush_node_get_absolute_path_out_path = NULL;
        ush_node_get_absolute_path_call_count = 0;

        ush_node_get_parent_by_path_path = NULL;
        ush_node_get_parent_by_path_call_count = 0;
        ush_node_get_parent_by_path_return_val = NULL;

        ush_write_pointer_call_count = 0;
        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;

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

void ush_node_get_absolute_path(struct ush_object *self, const char *in_path, char *out_path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_absolute_path_in_path, in_path);

        strcpy(out_path, ush_node_get_absolute_path_out_path);
        ush_node_get_absolute_path_call_count++;
}

struct ush_node_object* ush_node_get_parent_by_path(struct ush_object *self, const char *path)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_node_get_parent_by_path_path, path);

        ush_node_get_parent_by_path_call_count++;
        return ush_node_get_parent_by_path_return_val;
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

void test_ush_autocomp_check_for_finish(void)
{
        for (int i = 0; i < 10; i++) {
                setUp();
                ush.process_stage = i;

                switch (i) {
                case 0:
                        ush.current_node = (struct ush_node_object*)1234;
                        ush.process_node = NULL;
                        ush.process_index = 10;
                        ush.process_index_item = 20;
                        ush_autocomp_check_for_finish(&ush);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(1, ush.process_stage);
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
                        break;
                case 1:
                        setUp();
                        ush.process_stage = i;

                        struct ush_node_object node = {0};
                        node.childs = (struct ush_node_object*)1234;

                        ush.process_index = 10;
                        ush.process_index_item = 20;
                        ush.autocomp_input = "test";
                        ush_node_get_absolute_path_in_path = "test";
                        ush_node_get_absolute_path_out_path = "qwerty";
                        ush_node_get_parent_by_path_path = "qwerty";
                        ush_node_get_parent_by_path_return_val = &node;
                        ush_autocomp_check_for_finish(&ush);
                        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);

                        TEST_ASSERT_EQUAL(2, ush.process_stage);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);

                        setUp();
                        ush.process_stage = i;

                        struct ush_node_object node_current = {0};
                        node_current.childs = (struct ush_node_object*)5678;

                        ush.process_index = 10;
                        ush.process_index_item = 20;
                        ush.autocomp_input = "";
                        ush.current_node = &node_current;
                        ush_node_get_absolute_path_in_path = "";
                        ush_node_get_absolute_path_out_path = "qwerty";
                        ush_autocomp_check_for_finish(&ush);
                        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);

                        TEST_ASSERT_EQUAL(2, ush.process_stage);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)5678, ush.process_node);
                        
                        setUp();
                        ush.process_stage = i;

                        ush.process_index = 10;
                        ush.process_index_item = 20;
                        ush.autocomp_input = "test";
                        ush.current_node = &node_current;
                        ush_node_get_absolute_path_in_path = "test";
                        ush_node_get_absolute_path_out_path = "qwerty";
                        ush_node_get_parent_by_path_path = "qwerty";
                        ush_node_get_parent_by_path_return_val = NULL;
                        ush_autocomp_check_for_finish(&ush);
                        TEST_ASSERT_EQUAL(1, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(1, ush_node_get_parent_by_path_call_count);

                        TEST_ASSERT_EQUAL(2, ush.process_stage);
                        TEST_ASSERT_EQUAL(0, ush.process_index);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)5678, ush.process_node);
                        break;
                case 2:
                        for (int s = 0; s < USH_STATE__TOTAL_NUM; s++) {
                                setUp();
                                ush.process_stage = i;
                                ush.state = (ush_state_t)s;
                                ush_autocomp_check_for_finish(&ush);
                                TEST_ASSERT_EQUAL((ush_state_t)s, ush.autocomp_prev_state);
                                TEST_ASSERT_EQUAL(USH_STATE_AUTOCOMP_CANDIDATES_FINISH, ush.state);
                                TEST_ASSERT_EQUAL(0, g_assert_call_count);
                                TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                                TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
                        }
                        break;
                default:
                        ush_autocomp_check_for_finish(&ush);
                        TEST_ASSERT_EQUAL(1, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_absolute_path_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_get_parent_by_path_call_count);
                        break;
                }
        }
}

void test_ush_autocomp_check_for_next(void)
{
        struct ush_node_object node = {0};
        node.file_list_size = 1;
        node.next = (struct ush_node_object*)1234;

        for (int i = 0; i < 10; i++) {
                setUp();
                ush.process_stage = i;
                ush.process_node = &node;

                switch (i) {
                case 0:
                        setUp();
                        ush.process_stage = i;
                        ush.process_node = &node;
                        TEST_ASSERT_FALSE(ush_autocomp_check_for_next(&ush));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(&node, ush.process_node);

                        setUp();
                        ush.process_stage = i;
                        ush.process_node = &node;
                        ush.process_index_item = 1;
                        TEST_ASSERT_TRUE(ush_autocomp_check_for_next(&ush));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.process_node);
                        break;
                case 1:
                        setUp();
                        ush.process_stage = i;
                        ush.process_node = &node;
                        TEST_ASSERT_FALSE(ush_autocomp_check_for_next(&ush));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(&node, ush.process_node);

                        setUp();
                        ush.process_stage = i;
                        ush.process_node = &node;
                        ush.process_index_item = 1;
                        TEST_ASSERT_TRUE(ush_autocomp_check_for_next(&ush));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        TEST_ASSERT_EQUAL(0, ush.process_index_item);
                        TEST_ASSERT_EQUAL(NULL, ush.process_node);
                        break;
                case 2:
                        TEST_ASSERT_FALSE(ush_autocomp_check_for_next(&ush));
                        TEST_ASSERT_EQUAL(0, g_assert_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_autocomp_check_for_next(&ush));
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

        RUN_TEST(test_ush_autocomp_start);
        RUN_TEST(test_ush_autocomp_prepare_candidates);
        RUN_TEST(test_ush_autocomp_optimize_continue);
        RUN_TEST(test_ush_autocomp_check_for_finish);
        RUN_TEST(test_ush_autocomp_check_for_next);

        return UNITY_END();
}
