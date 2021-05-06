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

int g_assert_call_count;

struct ush_object ush;

void file_process_service(struct ush_object *self, struct ush_file_descriptor const *file);

struct ush_file_descriptor ush_file;

int file_process_service_call_count;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        file_process_service_call_count = 0;
}

void tearDown(void)
{

}

void file_process_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(&ush_file, file);

        file_process_service_call_count++;
}

void test_ush_process_start(void)
{
        struct ush_file_descriptor *file_1 = (struct ush_file_descriptor *)1234;
        struct ush_file_descriptor *file_2 = (struct ush_file_descriptor *)5678;

        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.state = state;

                ush_process_start(&ush, file_1);
                TEST_ASSERT_EQUAL(file_1, ush.process_file);
                TEST_ASSERT_EQUAL(USH_STATE_PROCESS_START, ush.state);
                TEST_ASSERT_EQUAL(0, file_process_service_call_count);

                setUp();
                ush.state = state;

                ush_process_start(&ush, file_2);
                TEST_ASSERT_EQUAL(file_2, ush.process_file);
                TEST_ASSERT_EQUAL(USH_STATE_PROCESS_START, ush.state);
                TEST_ASSERT_EQUAL(0, file_process_service_call_count);
        }
}

void test_ush_process_service(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                ush.process_file = &ush_file;
                ush_file.process = file_process_service;
                ush.state = (ush_state_t)i;

                switch (state) {
                case USH_STATE_PROCESS_START:
                case USH_STATE_PROCESS_SERVICE:
                case USH_STATE_PROCESS_FINISH:
                        TEST_ASSERT_TRUE(ush_process_service(&ush));
                        TEST_ASSERT_EQUAL(state, ush.state);
                        TEST_ASSERT_EQUAL(1, file_process_service_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_process_service(&ush));
                        TEST_ASSERT_EQUAL(state, ush.state);
                        TEST_ASSERT_EQUAL(0, file_process_service_call_count);
                        break;
                }

                setUp();
                ush.process_file = &ush_file;
                ush_file.process = NULL;
                ush.state = (ush_state_t)i;

                switch (state) {
                case USH_STATE_PROCESS_START:
                case USH_STATE_PROCESS_SERVICE:
                case USH_STATE_PROCESS_FINISH:
                        TEST_ASSERT_TRUE(ush_process_service(&ush));
                        TEST_ASSERT_EQUAL(USH_STATE_RESET, ush.state);
                        TEST_ASSERT_EQUAL(0, file_process_service_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_process_service(&ush));
                        TEST_ASSERT_EQUAL(state, ush.state);
                        TEST_ASSERT_EQUAL(0, file_process_service_call_count);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_process_start);
        RUN_TEST(test_ush_process_service);

        return UNITY_END();
}
