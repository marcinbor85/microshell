#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"
#include "ush_const.h"
#include "ush.h"

int g_assert_call_count;

struct ush_object ush;

int ush_write_pointer_call_count;
int ush_prompt_start_call_count;

char *ush_write_pointer_text;
ush_state_t ush_write_pointer_state;
ush_state_t ush_prompt_start_state;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_write_pointer_call_count = 0;
        ush_prompt_start_call_count = 0;

        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;
        ush_prompt_start_state = USH_STATE__TOTAL_NUM;
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

void ush_prompt_start(struct ush_object *self, ush_state_t prepare_next_state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_prompt_start_state, prepare_next_state);

        ush_prompt_start_call_count++;
}

void test_ush_reset(void)
{
        ush_write_pointer_text = USH_NAME " " USH_VERSION "\r\n";
        ush_write_pointer_state = USH_STATE_RESET_PROMPT;
        ush.root = (struct ush_node_object*)1234;
        ush_reset(&ush);
        TEST_ASSERT_EQUAL((struct ush_node_object*)1234, ush.current_node);
        TEST_ASSERT_EQUAL(USH_STATE_RESET, ush.state);
        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_prompt_start_call_count);
}

void test_ush_reset_start(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.state = state;
                
                ush_reset_start(&ush);
                TEST_ASSERT_EQUAL(USH_STATE_RESET, ush.state);
                TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                TEST_ASSERT_EQUAL(0, ush_prompt_start_call_count);
        }
}

void test_ush_reset_service(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.state = state;

                switch (state) {
                case USH_STATE_RESET:
                        ush_write_pointer_text = "\r\n";
                        ush_write_pointer_state = USH_STATE_RESET_PROMPT;
                        TEST_ASSERT_TRUE(ush_reset_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_prompt_start_call_count);
                        break;
                case USH_STATE_RESET_PROMPT:
                        ush_prompt_start_state = USH_STATE_READ_PREPARE;
                        TEST_ASSERT_TRUE(ush_reset_service(&ush));
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_prompt_start_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_reset_service(&ush));
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_prompt_start_call_count);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_reset);
        RUN_TEST(test_ush_reset_start);
        RUN_TEST(test_ush_reset_service);

        return UNITY_END();
}
