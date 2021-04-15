#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"
#include "ush_shell.h"

struct ush_descriptor ush_desc;
struct ush_node_object ush_current_node;
struct ush_object ush;
bool ush_write_pointer_call_flag;
bool ush_utils_path_last_call_flag;

void setUp(void)
{
        ush_desc.hostname = "test_hostname";
        ush_current_node.path = "test_node_1";

        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush.desc = &ush_desc;
        ush.current_node = &ush_current_node;
        ush.prompt_next_state = USH_STATE__TOTAL_NUM;

        ush_write_pointer_call_flag = false;
        ush_utils_path_last_call_flag = false;
}

void tearDown(void)
{

}

void ush_utils_path_last(const char *in_path, char* *out_path)
{
        TEST_ASSERT_EQUAL_STRING("test_node_1", in_path);
        *out_path = "test_node_2";
        ush_utils_path_last_call_flag = true;
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        switch (self->state) {
        case USH_STATE_PROMPT_PREFIX:
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING(USH_SHELL_FONT_COLOR_MAGENTA "[", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_HOST, state);
                TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                ush_write_pointer_call_flag = true;
                break;
        case USH_STATE_PROMPT_HOST:
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING("test_hostname", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_SPACE, state);
                TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                ush_write_pointer_call_flag = true;
                break;
        case USH_STATE_PROMPT_SPACE:
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING(" ", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_PATH, state);
                TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                ush_write_pointer_call_flag = true;
                break;
        case USH_STATE_PROMPT_PATH:
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING("test_node_2", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_SUFFIX, state);
                TEST_ASSERT_TRUE(ush_utils_path_last_call_flag);
                ush_write_pointer_call_flag = true;
                break;
        case USH_STATE_PROMPT_SUFFIX:
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING("]$ " USH_SHELL_FONT_STYLE_RESET, text);
                TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, state);
                TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                ush_write_pointer_call_flag = true;
                break;
        default:
                TEST_ASSERT_FALSE(ush_prompt_service(&ush));
                TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                break;
        }
}

void test_ush_prompt_start(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();

                ush_state_t state = (ush_state_t)i;
                ush.state = state;

                ush_prompt_start(&ush, state);

                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_PREFIX, ush.state);
                TEST_ASSERT_EQUAL(state, ush.prompt_next_state);
        }
}

void test_ush_prompt_service(void)
{
       for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();

                ush_state_t state = (ush_state_t)i;
                ush.state = state;

                switch (ush.state) {
                case USH_STATE_PROMPT_PREFIX:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_TRUE(ush_write_pointer_call_flag);
                        TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                        break;
                case USH_STATE_PROMPT_HOST:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_TRUE(ush_write_pointer_call_flag);
                        TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                        break;
                case USH_STATE_PROMPT_SPACE:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_TRUE(ush_write_pointer_call_flag);
                        TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                        break;
                case USH_STATE_PROMPT_PATH:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_TRUE(ush_write_pointer_call_flag);
                        TEST_ASSERT_TRUE(ush_utils_path_last_call_flag);
                        break;
                case USH_STATE_PROMPT_SUFFIX:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_TRUE(ush_write_pointer_call_flag);
                        TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_prompt_service(&ush));
                        TEST_ASSERT_FALSE(ush_write_pointer_call_flag);
                        TEST_ASSERT_FALSE(ush_utils_path_last_call_flag);
                        break;
                }
       }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_prompt_start);
        RUN_TEST(test_ush_prompt_service);

        return UNITY_END();
}
