/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "inc/ush_internal.h"
#include "inc/ush_shell.h"

int g_assert_call_count;

struct ush_descriptor ush_desc;
struct ush_node_object ush_current_node;
struct ush_object ush;
int ush_write_pointer_call_count;
int ush_utils_path_last_call_count;

void setUp(void)
{
        ush_desc.hostname = "test_hostname";
        ush_current_node.path = "test_node_1";

        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.path_max_length = 256;
        ush.desc = &ush_desc;
        ush.current_node = &ush_current_node;
        ush.prompt_next_state = USH_STATE__TOTAL_NUM;

        ush_write_pointer_call_count = 0;
        ush_utils_path_last_call_count = 0;
}

void tearDown(void)
{

}

char* ush_utils_path_last(const char *in_path)
{
        TEST_ASSERT_EQUAL_STRING("test_node_1", in_path);

        ush_utils_path_last_call_count++;
        return "test_node_2";
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        if (self->state == USH_STATE_PROMPT_PREFIX) {
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING(USH_SHELL_FONT_COLOR_MAGENTA "[", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_HOST, state);
        } else if (self->state == USH_STATE_PROMPT_HOST) {
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING("test_hostname", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_SPACE, state);
        } else if (self->state == USH_STATE_PROMPT_SPACE) {
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING(" ", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_PATH, state);
        } else if (self->state == USH_STATE_PROMPT_PATH) {
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING("test_node_2", text);
                TEST_ASSERT_EQUAL(USH_STATE_PROMPT_SUFFIX, state);
        } else if (self->state == USH_STATE_PROMPT_SUFFIX) {
                TEST_ASSERT_EQUAL(&ush, self);
                TEST_ASSERT_EQUAL_STRING("]$ " USH_SHELL_FONT_STYLE_RESET, text);
                TEST_ASSERT_EQUAL(USH_STATE__TOTAL_NUM, state);
        }

        ush_write_pointer_call_count++;
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

                switch (state) {
                case USH_STATE_PROMPT_PREFIX:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_path_last_call_count);
                        break;
                case USH_STATE_PROMPT_HOST:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_path_last_call_count);
                        break;
                case USH_STATE_PROMPT_SPACE:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_path_last_call_count);
                        break;
                case USH_STATE_PROMPT_PATH:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_utils_path_last_call_count);
                        break;
                case USH_STATE_PROMPT_SUFFIX:
                        TEST_ASSERT_TRUE(ush_prompt_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_path_last_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_prompt_service(&ush));
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_path_last_call_count);
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
