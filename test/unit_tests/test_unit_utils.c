#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "ush_utils.h"

int g_assert_call_count;

void setUp(void)
{
        
}

void tearDown(void)
{

}

void test_ush_utils_get_path_levels_count(void)
{
        TEST_ASSERT_EQUAL(0, ush_utils_get_path_levels_count(""));
        TEST_ASSERT_EQUAL(0, ush_utils_get_path_levels_count("/"));
        TEST_ASSERT_EQUAL(2, ush_utils_get_path_levels_count(" /"));
        TEST_ASSERT_EQUAL(2, ush_utils_get_path_levels_count(" / "));
        TEST_ASSERT_EQUAL(1, ush_utils_get_path_levels_count("test"));
        TEST_ASSERT_EQUAL(2, ush_utils_get_path_levels_count("test/"));  
        TEST_ASSERT_EQUAL(2, ush_utils_get_path_levels_count("test/abc"));
        TEST_ASSERT_EQUAL(3, ush_utils_get_path_levels_count("test/abc/"));
        TEST_ASSERT_EQUAL(1, ush_utils_get_path_levels_count("/test"));
        TEST_ASSERT_EQUAL(2, ush_utils_get_path_levels_count("/test/"));
        TEST_ASSERT_EQUAL(2, ush_utils_get_path_levels_count("/test/abc"));
        TEST_ASSERT_EQUAL(3, ush_utils_get_path_levels_count("/test/abc/"));
        TEST_ASSERT_EQUAL(3, ush_utils_get_path_levels_count("/test/../qwerty"));
        TEST_ASSERT_EQUAL(4, ush_utils_get_path_levels_count("/test/../qwerty/"));
        TEST_ASSERT_EQUAL(3, ush_utils_get_path_levels_count("/test/../qwerty"));
        TEST_ASSERT_EQUAL(4, ush_utils_get_path_levels_count("/test/../qwerty/"));
}

#define TEST_STRING_PROCESS(expected, input, func) { \
        memset(out, 0, sizeof(out)); \
        strcpy(in, input); \
        func(in, out); \
        TEST_ASSERT_EQUAL_STRING(expected, out); \
}

void test_ush_utils_get_collapse_path(void)
{
        char out[128] = {0};
        char in[128] = {0};

        TEST_STRING_PROCESS("/", "", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "/", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "test", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "test/", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test", "/test", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test/", "/test/", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/abc", "test/abc", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/abc/", "test/abc/", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test/abc", "/test/abc", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test/abc/", "/test/abc/", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test", "../test", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test", "/../test", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "/test/..", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "../test/..", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "../test/../", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/abc", "../test/../abc", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/abc/", "../test/../abc/", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/", "./", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test", "./test", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test/abc", "/test/./abc", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test//", "/test//", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test//abc", "/test//abc", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("/test///abc", "/test///abc", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("//", "//", ush_utils_get_collapse_path);
        TEST_STRING_PROCESS("///test", "///test", ush_utils_get_collapse_path);
}

#define TEST_STRING_CHANGE(expected, input, func) { \
        memset(out, 0, sizeof(out)); \
        strcpy(out, input); \
        func(out); \
        TEST_ASSERT_EQUAL_STRING(expected, out); \
}

void test_ush_utils_path_upper(void)
{
        char out[128] = {0};

        TEST_STRING_CHANGE("", "", ush_utils_path_upper);
        TEST_STRING_CHANGE("", "/", ush_utils_path_upper);
        TEST_STRING_CHANGE("/", "/test", ush_utils_path_upper);
        TEST_STRING_CHANGE("/test", "/test/", ush_utils_path_upper);
        TEST_STRING_CHANGE("test", "test", ush_utils_path_upper);
        TEST_STRING_CHANGE("test", "test/", ush_utils_path_upper);
        TEST_STRING_CHANGE("test", "test/abc", ush_utils_path_upper);
        TEST_STRING_CHANGE("test/abc", "test/abc/xyz", ush_utils_path_upper);
        TEST_STRING_CHANGE("/test", "/test/abc", ush_utils_path_upper);
        TEST_STRING_CHANGE("/test/abc", "/test/abc/", ush_utils_path_upper);
        TEST_STRING_CHANGE("/test/abc", "/test/abc/qwerty", ush_utils_path_upper);
}

#define TEST_STRING_PROCESS_PTR(expected, input, func) { \
        strcpy(in, input); \
        func(in, &out); \
        TEST_ASSERT_EQUAL_STRING(expected, out); \
}

void test_ush_utils_path_last(void)
{
        char *out;
        char in[128] = {0};

        TEST_STRING_PROCESS_PTR("", "", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("/", "/", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("test", "/test", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("", "test/", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("abc", "test/abc", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("abc", "/test/abc", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("abc", "xyz/test/abc", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("qwerty", "/test/abc/qwerty", ush_utils_path_last);
        TEST_STRING_PROCESS_PTR("", "xyz/test/abc/", ush_utils_path_last);
}

#define TEST_STRING_PROCESS_ARGS(expected, arg1, input, func) { \
        memset(out, 0, sizeof(out)); \
        strcpy(in, input); \
        func(arg1, in, out); \
        TEST_ASSERT_EQUAL_STRING(expected, out); \
}

void test_ush_utils_get_path_level(void)
{
        char out[128] = {0};
        char in[128] = {0};

        TEST_STRING_PROCESS_ARGS("/", 0, "", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("", 1, "", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("", 2, "", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "test", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test", 1, "test", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test", 2, "test", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "test/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test", 1, "test/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test/", 2, "test/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "test/abc", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test", 1, "test/abc", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test/abc", 2, "test/abc", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "test/abc/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test", 1, "test/abc/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("test/abc", 2, "test/abc/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "/test", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test", 1, "/test", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test", 2, "/test", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "/test/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test", 1, "/test/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test/", 2, "/test/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "/test/abc", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test", 1, "/test/abc", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test/abc", 2, "/test/abc", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/", 0, "/test/abc/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test", 1, "/test/abc/", ush_utils_get_path_level);
        TEST_STRING_PROCESS_ARGS("/test/abc", 2, "/test/abc/", ush_utils_get_path_level);
}

void test_ush_utils_join_path(void)
{
        char out[128] = {0};
        char in[128] = {0};

        TEST_STRING_PROCESS_ARGS("/", "", "", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("//", "", "/", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("//test", "", "/test", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/test", "", "test", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/", "/", "", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/abc", "/", "abc", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("//abc", "/", "/abc", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/abc/", "/", "abc/", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/test/", "/test", "", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("test/", "test", "", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/test/abc", "/test", "abc", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/test//abc", "/test", "/abc", ush_utils_join_path);
        TEST_STRING_PROCESS_ARGS("/test//abc", "/test/", "abc", ush_utils_join_path);
}

#define TEST_STRING_PROCESS_RET(expected, in, func) { \
        char *out = func(in); \
        TEST_ASSERT_EQUAL_STRING(expected, out); \
}

void test_ush_utils_get_last_arg(void)
{
        TEST_STRING_PROCESS_RET("", "", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("", "   ", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("test", "test", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("test", "   test", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("", "test ", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("abc", "test abc", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("abc", "test    abc", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("", "test    abc  ", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("xyz", "test abc xyz", ush_utils_get_last_arg);
        TEST_STRING_PROCESS_RET("", "test abc xyz   ", ush_utils_get_last_arg);
}

void test_ush_utils_startswith(void)
{
        TEST_ASSERT_TRUE(ush_utils_startswith("test", ""));
        TEST_ASSERT_TRUE(ush_utils_startswith("test", "te"));
        TEST_ASSERT_TRUE(ush_utils_startswith("test", "test"));
        TEST_ASSERT_FALSE(ush_utils_startswith("test", "test/"));
        TEST_ASSERT_TRUE(ush_utils_startswith("", ""));
        TEST_ASSERT_FALSE(ush_utils_startswith("", "te"));
        TEST_ASSERT_FALSE(ush_utils_startswith("", "test"));
        TEST_ASSERT_FALSE(ush_utils_startswith(" test", "test"));
        TEST_ASSERT_FALSE(ush_utils_startswith("test", " test"));
}

void test_ush_utils_is_printable(void)
{
        for (int i = 0; i < 256; i++) {
                if (i < 0x20 || i > 0x7E) {
                        TEST_ASSERT_FALSE(ush_utils_is_printable((char)i));
                } else {
                        TEST_ASSERT_TRUE(ush_utils_is_printable((char)i));
                }
        }
}

void test_ush_utils_get_status_string(void)
{
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_OK, ush_utils_get_status_string(USH_STATUS_OK));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_DIRECTORY_NOT_FOUND, ush_utils_get_status_string(USH_STATUS_ERROR_NODE_NOT_FOUND));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_NESTED_DIRECTORIES_EXIST, ush_utils_get_status_string(USH_STATUS_ERROR_NODE_WITH_CHILDS));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_CANNOT_FIND_PARENT_NODE, ush_utils_get_status_string(USH_STATUS_ERROR_NODE_WITHOUT_PARENT));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_DIRECTORY_ALREADY_MOUNTED, ush_utils_get_status_string(USH_STATUS_ERROR_NODE_ALREADY_MOUNTED));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_SYNTAX_ERROR, ush_utils_get_status_string(USH_STATUS_ERROR_COMMAND_SYNTAX_ERROR));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_WRONG_ARGUMENTS, ush_utils_get_status_string(USH_STATUS_ERROR_COMMAND_WRONG_ARGUMENTS));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_FILE_NOT_EXECUTABLE, ush_utils_get_status_string(USH_STATUS_ERROR_FILE_NOT_EXECUTABLE));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_NO_HELP_AVAILABLE, ush_utils_get_status_string(USH_STATUS_ERROR_FILE_NO_HELP));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_FILE_NOT_FOUND, ush_utils_get_status_string(USH_STATUS_ERROR_FILE_NOT_FOUND));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_NO_DATA_AVAILABLE, ush_utils_get_status_string(USH_STATUS_ERROR_FILE_NO_DATA));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR ": " USH_CONFIG_TRANSLATION_READ_ONLY_FILE, ush_utils_get_status_string(USH_STATUS_ERROR_FILE_READ_ONLY));
        TEST_ASSERT_EQUAL_STRING(USH_CONFIG_TRANSLATION_ERROR, ush_utils_get_status_string(-1));
}

#define TEST_UINT8_PROCESS_ARGS(expected, expected_size, input, func) { \
        memset(out, 0, sizeof(out)); \
        func(input, out, sizeof(out)); \
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, out, expected_size); \
}


void test_ush_utils_decode_ascii(void)
{
        uint8_t out[8] = {0};

        TEST_UINT8_PROCESS_ARGS("abc", 3, "abc", ush_utils_decode_ascii);
        TEST_UINT8_PROCESS_ARGS("abc", 3, "\\a\\b\\c", ush_utils_decode_ascii);
        TEST_UINT8_PROCESS_ARGS("\x01""a\xFF", 3, "\\x01a\\xFF", ush_utils_decode_ascii);
        TEST_UINT8_PROCESS_ARGS("abcd\x00\x01\x02\x03", 8, "abcd\\x00\\x01\\x02\\x03", ush_utils_decode_ascii);
        TEST_UINT8_PROCESS_ARGS("\xde\xad\xbe\xef", 4, "\\xde\\xad\\xbe\\xef", ush_utils_decode_ascii);
        TEST_UINT8_PROCESS_ARGS("abcdqwert", 8, "abcdqwertyuiop", ush_utils_decode_ascii);

        for (int i = 0; i < 256; i++) {
                char in[32];
                sprintf(in, "\\x%02X\\x%02x", i, i);
                char exp[2];
                exp[0] = i;
                exp[1] = i;
                TEST_UINT8_PROCESS_ARGS(exp, 2, in, ush_utils_decode_ascii);
        }

        for (int i = 0; i < 256; i++) {
                char in[32];
                if ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'F') || (i >= 'a' && i <= 'f') || (i == '\0'))
                        continue;
                
                for (int n = 0; n < 16; n++) {
                        sprintf(in, "\\x%c%1X\\x%1X%c", i, n, n, i);
                        char exp[2];
                        exp[0] = n;
                        exp[1] = n << 4;
                        TEST_UINT8_PROCESS_ARGS(exp, 2, in, ush_utils_decode_ascii);
                }                
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_utils_get_path_levels_count);
        RUN_TEST(test_ush_utils_get_collapse_path);
        RUN_TEST(test_ush_utils_path_upper);
        RUN_TEST(test_ush_utils_path_last);
        RUN_TEST(test_ush_utils_get_path_level);
        RUN_TEST(test_ush_utils_join_path);
        RUN_TEST(test_ush_utils_get_last_arg);
        RUN_TEST(test_ush_utils_startswith);
        RUN_TEST(test_ush_utils_is_printable);
        RUN_TEST(test_ush_utils_get_status_string);
        RUN_TEST(test_ush_utils_decode_ascii);

        return UNITY_END();
}
