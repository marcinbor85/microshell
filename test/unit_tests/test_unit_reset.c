#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"

struct ush_object ush;
bool ush_write_pointer_call_flag;
bool ush_prompt_start_call_flag;
ush_state_t init_state;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_write_pointer_call_flag = false;
        ush_prompt_start_call_flag = false;
}

void tearDown(void)
{

}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING("\r\n", text);
        TEST_ASSERT_EQUAL(USH_STATE_RESET_PROMPT, state);

        ush_write_pointer_call_flag = true;
}

void ush_prompt_start(struct ush_object *self, ush_state_t prepare_next_state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(USH_STATE_READ_PREPARE, prepare_next_state);

        ush_prompt_start_call_flag = true;
}

void test_ush_reset_start(void)
{
        ush.state = USH_STATE__TOTAL_NUM;
        ush_reset_start(&ush);
        TEST_ASSERT_EQUAL(USH_STATE_RESET, ush.state);
}

void test_ush_reset_service(void)
{
        ush.state = init_state;
        switch (ush.state) {
        case USH_STATE_RESET:
                TEST_ASSERT_TRUE(ush_reset_service(&ush));
                TEST_ASSERT_TRUE(ush_write_pointer_call_flag);
                TEST_ASSERT_FALSE(ush_prompt_start_call_flag);
                break;
        case USH_STATE_RESET_PROMPT:
                TEST_ASSERT_TRUE(ush_reset_service(&ush));
                TEST_ASSERT_FALSE(ush_write_pointer_call_flag);
                TEST_ASSERT_TRUE(ush_prompt_start_call_flag);
                break;
        default:
                TEST_ASSERT_FALSE(ush_reset_service(&ush));
                TEST_ASSERT_FALSE(ush_write_pointer_call_flag);
                TEST_ASSERT_FALSE(ush_prompt_start_call_flag);
                break;
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_reset_start);
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                init_state = (ush_state_t)i;
                RUN_TEST(test_ush_reset_service);
        }

        return UNITY_END();
}
