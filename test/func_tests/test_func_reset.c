#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ush.h"
#include "ush_const.h"
#include "test_func.h"

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{
        test_func_deinit();
}

void test_reset(void)
{
        ush_reset(&g_ush);
        test_func_read_all();
        TEST_ASSERT_EQUAL_STRING(USH_NAME " " USH_VERSION "\r\n[test /]$ ", g_write_buf);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_reset);

        return UNITY_END();
}
