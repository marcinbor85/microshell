#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ush.h"
#include "test_func.h"

void setUp(void)
{
        test_func_init();
}

void tearDown(void)
{

}

void test_cmd_cat_single(void)
{
        sprintf((char*)g_file_buffer_buf, "test_data");
        g_file_buffer_buf_size = 9;
        TEST_FUNC_ASK("cat /data/buffer",
                "test_data[test /]$ "
        );

        sprintf((char*)g_file_buffer_buf, "\x01\x02""abcd\x03\xFF");
        g_file_buffer_buf_size = 8;
        TEST_FUNC_ASK("cat /data/buffer",
                "\x01\x02""abcd\x03\xFF""[test /]$ "
        );

        TEST_FUNC_ASK("cat /data/null",
                "[test /]$ "
        );
}

void test_cmd_cat_multi(void)
{
        sprintf((char*)g_file_buffer_buf, "test");
        g_file_buffer_buf_size = 4;
        TEST_FUNC_ASK("cat /data/buffer /data/buffer data/../dir/../data/buffer",
                "testtesttest[test /]$ "
        );

        ush_node_set_current_dir(&g_ush, "/data");

        sprintf((char*)g_file_buffer_buf, "abcd");
        g_file_buffer_buf_size = 4;
        TEST_FUNC_ASK("cat /data/buffer ./binary",
                "abcd"
                "\xFF\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
                "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
                "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
                "\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
                "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
                "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF"
                "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF"
                "\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
                "\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF"
                "[test data]$ "
        );

        ush_node_set_current_dir(&g_ush, "/dir");

        g_file_buffer_buf[0] = '\0';
        g_file_buffer_buf[1] = 'T';
        g_file_buffer_buf_size = 2;

        uint8_t buf_resp[TEST_FUNC_IO_BUFFER_SIZE];
        memset(buf_resp, 0, sizeof(buf_resp));
        strcpy((char*)buf_resp, "cat /data/binary ../data/buffer\n");
        for (size_t i = 0; i < 256; i++)
                buf_resp[32 + i] = (i == 0) ? 0xFF : i;
        buf_resp[32 + 256 + 0] = '\0';
        buf_resp[32 + 256 + 1] = 'T';
        strcpy((char*)&buf_resp[32 + 256 + 2], "[test dir]$ ");

        test_func_write("cat /data/binary ../data/buffer\n");
        test_func_read_all();
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buf_resp, g_write_buf, (32 + 256 + 2 + 12));
}

void test_cmd_cat_help(void)
{
        TEST_FUNC_ASK("help cat",
                "cat: cat [file...]\r\n"
                "\tPrint concatenate text files content.\r\n"
                "[test /]$ "
        );
}

void test_cmd_cat_error(void)
{
        TEST_FUNC_ASK("cat",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("cat 1",
                "error: file not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("cat test",
                "error: file not readable\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("cat /data/null 1",
                "error: file not found\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_cat_single);
        RUN_TEST(test_cmd_cat_multi);
        RUN_TEST(test_cmd_cat_help);
        RUN_TEST(test_cmd_cat_error);

        return UNITY_END();
}
