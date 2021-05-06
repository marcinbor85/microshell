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

struct ush_descriptor ush_desc;
struct ush_object ush;

int ush_write_pointer_bin_call_count;
int ush_write_char_call_count;

uint8_t *ush_write_pointer_bin_data;
size_t ush_write_pointer_bin_data_size;
ush_state_t ush_write_pointer_bin_write_next_state;

int write_callback(struct ush_object *self, char ch);

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.path_max_length = 256;
        ush.desc = &ush_desc;
        ush.write_buf = "test_message";

        ush_write_pointer_bin_call_count = 0;
        ush_write_char_call_count = 0;

        ush_write_pointer_bin_data = NULL;
        ush_write_pointer_bin_data_size = 0;
        ush_write_pointer_bin_write_next_state = USH_STATE__TOTAL_NUM;
}

void tearDown(void)
{

}

void ush_write_pointer_bin(struct ush_object *self, uint8_t *data, size_t data_size, ush_state_t write_next_state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_NOT_NULL(data);

        TEST_ASSERT_EQUAL(ush_write_pointer_bin_data, data);
        TEST_ASSERT_EQUAL(ush_write_pointer_bin_data_size, data_size);
        TEST_ASSERT_EQUAL(ush_write_pointer_bin_write_next_state, write_next_state);
        
        ush_write_pointer_bin_call_count++;
}

void ush_write_char(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_write_char_call_count++;
}

void test_ush_write_pointer(void)
{
        char *data_long = (char*)"test_string";
        char *data_short = (char*)"ush";
        
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                ush_state_t state = (ush_state_t)i;

                setUp();
                
                ush_write_pointer_bin_data = (uint8_t*)data_long;
                ush_write_pointer_bin_data_size = 11;
                ush_write_pointer_bin_write_next_state = state;
                ush_write_pointer(&ush, data_long, ush_write_pointer_bin_write_next_state);
                TEST_ASSERT_EQUAL(1, ush_write_pointer_bin_call_count);

                setUp();

                ush_write_pointer_bin_data = (uint8_t*)data_short;
                ush_write_pointer_bin_data_size = 3;
                ush_write_pointer_bin_write_next_state = state;
                ush_write_pointer(&ush, data_short, ush_write_pointer_bin_write_next_state);
                TEST_ASSERT_EQUAL(1, ush_write_pointer_bin_call_count);
        }
}

void test_ush_write_service(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();
                
                ush_state_t state = (ush_state_t)i;
                ush.state = state;

                switch (state) {
                case USH_STATE_WRITE_CHAR:
                        TEST_ASSERT_TRUE(ush_write_service(&ush));
                        TEST_ASSERT_EQUAL(1, ush_write_char_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_write_service(&ush));
                        TEST_ASSERT_EQUAL(0, ush_write_char_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_bin_call_count);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_write_pointer);
        RUN_TEST(test_ush_write_service);

        return UNITY_END();
}
