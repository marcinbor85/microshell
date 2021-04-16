#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_internal.h"

struct ush_object ush;

int ush_read_start_call_count;
int ush_read_char_call_count;
bool ush_read_char_return_val;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));

        ush_read_start_call_count = 0;
        ush_read_char_call_count = 0;

        ush_read_char_return_val = false;
}

void tearDown(void)
{

}

void ush_read_start(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_read_start_call_count++;
}

bool ush_read_char(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_read_char_call_count++;

        return ush_read_char_return_val;
}

void test_ush_read_service_state(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();

                ush.state = (ush_state_t)i;
                bool read;
                
                switch (ush.state) {
                case USH_STATE_READ_PREPARE:
                        TEST_ASSERT_TRUE(ush_read_service(&ush, &read));
                        TEST_ASSERT_EQUAL(1, ush_read_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_call_count);
                        break;
                case USH_STATE_READ_CHAR:
                        TEST_ASSERT_TRUE(ush_read_service(&ush, &read));
                        TEST_ASSERT_EQUAL(0, ush_read_start_call_count);
                        TEST_ASSERT_EQUAL(1, ush_read_char_call_count);
                        break;
                default:
                        TEST_ASSERT_FALSE(ush_read_service(&ush, &read));
                        TEST_ASSERT_EQUAL(0, ush_read_start_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_char_call_count);
                        break;
                }
        }
}

void test_ush_read_service_read(void)
{
        bool read;

        ush.state = USH_STATE_READ_CHAR;

        read = false;
        ush_read_char_return_val = true;
        TEST_ASSERT_TRUE(ush_read_service(&ush, &read));
        TEST_ASSERT_TRUE(read);

        read = true;
        ush_read_char_return_val = false;
        TEST_ASSERT_TRUE(ush_read_service(&ush, &read));
        TEST_ASSERT_FALSE(read);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_read_service_state);
        RUN_TEST(test_ush_read_service_read);

        return UNITY_END();
}
