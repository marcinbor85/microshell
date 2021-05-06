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

#include "inc/ush.h"

int g_assert_call_count;

struct ush_io_interface ush_io_iface;
struct ush_descriptor ush_desc;
struct ush_object ush;

const struct ush_file_descriptor g_ush_buildin_commands[3];
const size_t g_ush_buildin_commands_num = 3;

ush_status_t ush_commands_add_return_val;
int ush_commands_add_call_count;
int ush_reset_call_count;

int ush_write_pointer_call_count;
char *ush_write_pointer_text;
ush_state_t ush_write_pointer_state;

ush_status_t ush_utils_get_status_string_status;
char *ush_utils_get_status_string_return_val;
int ush_utils_get_status_string_call_count;

struct ush_node_object *ush_node_deinit_recursive_node;
int ush_node_deinit_recursive_call_count;

int ush_reset_service_call_count;
int ush_prompt_service_call_count;
int ush_read_service_call_count;
int ush_autocomp_service_call_count;
int ush_parse_service_call_count;
int ush_write_service_call_count;
int ush_process_service_call_count;

bool ush_reset_service_return_val;
bool ush_prompt_service_return_val;
bool ush_read_service_return_val;
bool ush_read_service_busy;
bool ush_autocomp_service_return_val;
bool ush_parse_service_return_val;
bool ush_write_service_return_val;
bool ush_process_service_return_val;

void setUp(void)
{
        memset((uint8_t*)&ush_desc, 0, sizeof(ush_desc));
        memset((uint8_t*)&ush, 0, sizeof(ush));
        ush_desc.path_max_length = 256;
        ush_desc.io = &ush_io_iface;

        ush_commands_add_return_val = USH_STATUS_OK;
        ush_commands_add_call_count = 0;
        ush_reset_call_count = 0;

        ush_write_pointer_call_count = 0;
        ush_write_pointer_text = NULL;
        ush_write_pointer_state = USH_STATE__TOTAL_NUM;

        ush_utils_get_status_string_status = USH_STATUS__TOTAL_NUM;
        ush_utils_get_status_string_return_val = NULL;
        ush_utils_get_status_string_call_count = 0;

        ush_node_deinit_recursive_node = NULL;
        ush_node_deinit_recursive_call_count = 0;

        ush_reset_service_call_count = 0;
        ush_prompt_service_call_count = 0;
        ush_read_service_call_count = 0;
        ush_autocomp_service_call_count = 0;
        ush_parse_service_call_count = 0;
        ush_write_service_call_count = 0;
        ush_process_service_call_count = 0;

        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;

        g_assert_call_count = 0;
}

void tearDown(void)
{

}

ush_status_t ush_commands_add(struct ush_object *self, struct ush_node_object *node, const struct ush_file_descriptor *file_list, size_t file_list_size)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(&ush.buildin_commands, node);
        TEST_ASSERT_EQUAL(g_ush_buildin_commands, file_list);
        TEST_ASSERT_EQUAL(3, file_list_size);

        ush_commands_add_call_count++;

        return ush_commands_add_return_val;
}

void ush_reset(struct ush_object *self)
{
        TEST_ASSERT_EQUAL(&ush, self);

        ush_reset_call_count++;
}

void ush_write_pointer(struct ush_object *self, char *text, ush_state_t state)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL_STRING(ush_write_pointer_text, text);
        TEST_ASSERT_EQUAL(ush_write_pointer_state, state);

        ush_write_pointer_call_count++;
}

const char* ush_utils_get_status_string(ush_status_t status)
{
        TEST_ASSERT_EQUAL(ush_utils_get_status_string_status, status);

        ush_utils_get_status_string_call_count++;

        return ush_utils_get_status_string_return_val;
}

void ush_node_deinit_recursive(struct ush_object *self, struct ush_node_object *node)
{
        TEST_ASSERT_EQUAL(&ush, self);
        TEST_ASSERT_EQUAL(ush_node_deinit_recursive_node, node);

        ush_node_deinit_recursive_call_count++;
}

bool ush_reset_service(struct ush_object *self)
{
        (void)self;
        ush_reset_service_call_count++;
        return ush_reset_service_return_val;
}

bool ush_prompt_service(struct ush_object *self)
{
        (void)self;
        ush_prompt_service_call_count++;
        return ush_prompt_service_return_val;
}

bool ush_read_service(struct ush_object *self, bool *busy)
{
        (void)self;
        (void)busy;
        ush_read_service_call_count++;
        *busy = ush_read_service_busy;
        return ush_read_service_return_val;
}

bool ush_autocomp_service(struct ush_object *self)
{
        (void)self;
        ush_autocomp_service_call_count++;
        return ush_autocomp_service_return_val;
}

bool ush_parse_service(struct ush_object *self)
{
        (void)self;
        ush_parse_service_call_count++;
        return ush_parse_service_return_val;
}

bool ush_write_service(struct ush_object *self)
{
        (void)self;
        ush_write_service_call_count++;
        return ush_write_service_return_val;
}

bool ush_process_service(struct ush_object *self)
{
        (void)self;
        ush_process_service_call_count++;
        return ush_process_service_return_val;
}

void test_ush_init(void)
{
        for (int s = 0; s < USH_STATUS__TOTAL_NUM; s++) {
                ush_status_t status = (ush_status_t)s;
                setUp();

                ush_commands_add_return_val = s;
                ush.root = (struct ush_node_object*)1234;
                ush_init(&ush, &ush_desc);
                switch (status) {
                case USH_STATUS_OK:
                        TEST_ASSERT_EQUAL(&ush_desc, ush.desc);
                        TEST_ASSERT_NULL(ush.root);
                        TEST_ASSERT_EQUAL(7, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_commands_add_call_count);
                        TEST_ASSERT_EQUAL(1, ush_reset_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
                        TEST_ASSERT_EQUAL(0, ush_reset_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
                        break;
                default:
                        TEST_ASSERT_EQUAL(&ush_desc, ush.desc);
                        TEST_ASSERT_NULL(ush.root);
                        TEST_ASSERT_EQUAL(8, g_assert_call_count);
                        TEST_ASSERT_EQUAL(1, ush_commands_add_call_count);
                        TEST_ASSERT_EQUAL(1, ush_reset_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
                        TEST_ASSERT_EQUAL(0, ush_reset_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
                        break;
                }
        }
}

void test_ush_deinit(void)
{
        memset((uint8_t*)&ush, 0xFF, sizeof(ush));
        ush.root = (struct ush_node_object*)1234;
        ush_node_deinit_recursive_node = (struct ush_node_object*)1234;
        ush_deinit(&ush);
        TEST_ASSERT_EACH_EQUAL_UINT8(0, (uint8_t*)&ush, sizeof(ush));
        TEST_ASSERT_EQUAL(1, ush_node_deinit_recursive_call_count);
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
}

void test_ush_service_none(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_FALSE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_all(void)
{
        ush_reset_service_return_val = true;
        ush_prompt_service_return_val = true;
        ush_read_service_return_val = true;
        ush_read_service_busy = true;
        ush_autocomp_service_return_val = true;
        ush_parse_service_return_val = true;
        ush_write_service_return_val = true;
        ush_process_service_return_val = true;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_reset(void)
{
        ush_reset_service_return_val = true;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_prompt(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = true;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_read(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = true;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_FALSE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);

        setUp();

        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = true;
        ush_read_service_busy = true;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_autocomp(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = true;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_parse(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = true;
        ush_write_service_return_val = false;
        ush_process_service_return_val = false;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_write(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = true;
        ush_process_service_return_val = false;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_service_process(void)
{
        ush_reset_service_return_val = false;
        ush_prompt_service_return_val = false;
        ush_read_service_return_val = false;
        ush_read_service_busy = false;
        ush_autocomp_service_return_val = false;
        ush_parse_service_return_val = false;
        ush_write_service_return_val = false;
        ush_process_service_return_val = true;
        TEST_ASSERT_TRUE(ush_service(&ush));
        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
        TEST_ASSERT_EQUAL(0, ush_write_pointer_call_count);
        TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
        TEST_ASSERT_EQUAL(1, ush_reset_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_prompt_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_read_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_autocomp_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_parse_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_write_service_call_count);
        TEST_ASSERT_EQUAL(1, ush_process_service_call_count);
        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
}

void test_ush_print_status(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                for (int n = 0; n < USH_STATUS__TOTAL_NUM; n++) {
                        setUp();

                        ush_state_t state = (ush_state_t)i;
                        ush.state = state;

                        ush_status_t status = (ush_status_t)n;

                        ush_write_pointer_text = "test";
                        ush_write_pointer_state = USH_STATE_RESET;
                        ush_utils_get_status_string_status = status;
                        ush_utils_get_status_string_return_val = "test";
                        ush_print_status(&ush, status);
                        TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
                        TEST_ASSERT_EQUAL(0, ush_reset_call_count);
                        TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                        TEST_ASSERT_EQUAL(1, ush_utils_get_status_string_call_count);
                        TEST_ASSERT_EQUAL(0, ush_reset_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
                        TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
                }
        }
}

void test_ush_print(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();

                ush_state_t state = (ush_state_t)i;
                ush.state = state;

                ush_write_pointer_text = "test";
                ush_write_pointer_state = USH_STATE_RESET;
                ush_print(&ush, "test");
                TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
                TEST_ASSERT_EQUAL(0, ush_reset_call_count);
                TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
                TEST_ASSERT_EQUAL(0, ush_reset_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
        }
}

void test_ush_print_no_newline(void)
{
        for (int i = 0; i < USH_STATE__TOTAL_NUM; i++) {
                setUp();

                ush_state_t state = (ush_state_t)i;
                ush.state = state;

                ush_write_pointer_text = "test";
                ush_write_pointer_state = USH_STATE_RESET_PROMPT;
                ush_print_no_newline(&ush, "test");
                TEST_ASSERT_EQUAL(0, ush_commands_add_call_count);
                TEST_ASSERT_EQUAL(0, ush_reset_call_count);
                TEST_ASSERT_EQUAL(1, ush_write_pointer_call_count);
                TEST_ASSERT_EQUAL(0, ush_utils_get_status_string_call_count);
                TEST_ASSERT_EQUAL(0, ush_reset_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_prompt_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_read_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_autocomp_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_parse_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_write_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_process_service_call_count);
                TEST_ASSERT_EQUAL(0, ush_node_deinit_recursive_call_count);
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_ush_init);
        RUN_TEST(test_ush_deinit);
        RUN_TEST(test_ush_service_none);
        RUN_TEST(test_ush_service_all);
        RUN_TEST(test_ush_service_reset);
        RUN_TEST(test_ush_service_prompt);
        RUN_TEST(test_ush_service_read);
        RUN_TEST(test_ush_service_autocomp);
        RUN_TEST(test_ush_service_parse);
        RUN_TEST(test_ush_service_write);
        RUN_TEST(test_ush_service_process);
        RUN_TEST(test_ush_print_status);
        RUN_TEST(test_ush_print);
        RUN_TEST(test_ush_print_no_newline);

        return UNITY_END();
}
