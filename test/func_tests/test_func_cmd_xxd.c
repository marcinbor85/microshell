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
        test_func_deinit();
}

void test_cmd_xxd(void)
{
        TEST_FUNC_ASK("xxd /data/null",
                "00000000:                                  \r\n"
                "[test /]$ "
        );

        sprintf((char*)g_file_buffer_buf, "abcdefg");
        g_file_buffer_buf_size = 7;
        TEST_FUNC_ASK("xxd /data/buffer",
                "00000000: 61 62 63 64 65 66 67     abcdefg \r\n"
                "[test /]$ "
        );

        sprintf((char*)g_file_buffer_buf, "abcdefgh");
        g_file_buffer_buf_size = 8;
        TEST_FUNC_ASK("xxd /data/buffer",
                "00000000: 61 62 63 64 65 66 67 68  abcdefgh\r\n"
                "[test /]$ "
        );

        sprintf((char*)g_file_buffer_buf, "abcdefghi");
        g_file_buffer_buf_size = 9;
        TEST_FUNC_ASK("xxd /data/buffer",
                "00000000: 61 62 63 64 65 66 67 68  abcdefgh\r\n"
                "00000008: 69                       i       \r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("xxd /data/binary",
                "00000000: 00 01 02 03 04 05 06 07  ........\r\n"
                "00000008: 08 09 0A 0B 0C 0D 0E 0F  ........\r\n"
                "00000010: 10 11 12 13 14 15 16 17  ........\r\n"
                "00000018: 18 19 1A 1B 1C 1D 1E 1F  ........\r\n"
                "00000020: 20 21 22 23 24 25 26 27   !\"#$%&'\r\n"
                "00000028: 28 29 2A 2B 2C 2D 2E 2F  ()*+,-./\r\n"
                "00000030: 30 31 32 33 34 35 36 37  01234567\r\n"
                "00000038: 38 39 3A 3B 3C 3D 3E 3F  89:;<=>?\r\n"
                "00000040: 40 41 42 43 44 45 46 47  @ABCDEFG\r\n"
                "00000048: 48 49 4A 4B 4C 4D 4E 4F  HIJKLMNO\r\n"
                "00000050: 50 51 52 53 54 55 56 57  PQRSTUVW\r\n"
                "00000058: 58 59 5A 5B 5C 5D 5E 5F  XYZ[\\]^_\r\n"
                "00000060: 60 61 62 63 64 65 66 67  `abcdefg\r\n"
                "00000068: 68 69 6A 6B 6C 6D 6E 6F  hijklmno\r\n"
                "00000070: 70 71 72 73 74 75 76 77  pqrstuvw\r\n"
                "00000078: 78 79 7A 7B 7C 7D 7E 7F  xyz{|}~.\r\n"
                "00000080: 80 81 82 83 84 85 86 87  ........\r\n"
                "00000088: 88 89 8A 8B 8C 8D 8E 8F  ........\r\n"
                "00000090: 90 91 92 93 94 95 96 97  ........\r\n"
                "00000098: 98 99 9A 9B 9C 9D 9E 9F  ........\r\n"
                "000000A0: A0 A1 A2 A3 A4 A5 A6 A7  ........\r\n"
                "000000A8: A8 A9 AA AB AC AD AE AF  ........\r\n"
                "000000B0: B0 B1 B2 B3 B4 B5 B6 B7  ........\r\n"
                "000000B8: B8 B9 BA BB BC BD BE BF  ........\r\n"
                "000000C0: C0 C1 C2 C3 C4 C5 C6 C7  ........\r\n"
                "000000C8: C8 C9 CA CB CC CD CE CF  ........\r\n"
                "000000D0: D0 D1 D2 D3 D4 D5 D6 D7  ........\r\n"
                "000000D8: D8 D9 DA DB DC DD DE DF  ........\r\n"
                "000000E0: E0 E1 E2 E3 E4 E5 E6 E7  ........\r\n"
                "000000E8: E8 E9 EA EB EC ED EE EF  ........\r\n"
                "000000F0: F0 F1 F2 F3 F4 F5 F6 F7  ........\r\n"
                "000000F8: F8 F9 FA FB FC FD FE FF  ........\r\n"
                "[test /]$ "
        );
}

void test_cmd_xxd_help(void)
{
        TEST_FUNC_ASK("help xxd",
                "xxd: xxd [file]\r\n"
                "\tShow file hex content.\r\n"
                "[test /]$ "
        );
}

void test_cmd_xxd_error(void)
{
        TEST_FUNC_ASK("xxd",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("xxd 1",
                "error: file not found\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("xxd test",
                "error: file not readable\r\n"
                "[test /]$ "
        );

        TEST_FUNC_ASK("xxd /data/null 1",
                "error: wrong arguments\r\n"
                "[test /]$ "
        );
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_cmd_xxd);
        RUN_TEST(test_cmd_xxd_help);
        RUN_TEST(test_cmd_xxd_error);

        return UNITY_END();
}
