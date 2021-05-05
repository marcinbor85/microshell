#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "inc/ush_commands.h"

int g_assert_call_count;

struct ush_object ush;

void setUp(void)
{
        memset((uint8_t*)&ush, 0, sizeof(ush));
}

void tearDown(void)
{

}

void ush_buildin_cmd_help_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void ush_buildin_cmd_help_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        (void)self;
        (void)file;
}

void ush_buildin_cmd_ls_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void ush_buildin_cmd_ls_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        (void)self;
        (void)file;
}

void ush_buildin_cmd_cd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void ush_buildin_cmd_pwd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void ush_buildin_cmd_cat_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void ush_buildin_cmd_cat_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        (void)self;
        (void)file;
}

void ush_buildin_cmd_xxd_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void ush_buildin_cmd_xxd_service(struct ush_object *self, struct ush_file_descriptor const *file)
{
        (void)self;
        (void)file;
}

void ush_buildin_cmd_echo_callback(struct ush_object *self, struct ush_file_descriptor const *file, int argc, char *argv[])
{
        (void)self;
        (void)file;
        (void)argc;
        (void)argv;
}

void test_g_ush_buildin_commands_help(void)
{
        struct ush_file_descriptor const *help = &g_ush_buildin_commands[0];

        TEST_ASSERT_EQUAL_STRING("help", help->name);
        TEST_ASSERT_NOT_NULL(help->description);
        TEST_ASSERT_NOT_NULL(help->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_help_callback, help->exec);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_help_service, help->process);
        TEST_ASSERT_NULL(help->get_data);
        TEST_ASSERT_NULL(help->set_data);
        help->exec(&ush, help, 0, NULL);
        help->process(&ush, help);
}

void test_g_ush_buildin_commands_ls(void)
{
        struct ush_file_descriptor const *ls = &g_ush_buildin_commands[1];

        TEST_ASSERT_EQUAL_STRING("ls", ls->name);
        TEST_ASSERT_NOT_NULL(ls->description);
        TEST_ASSERT_NOT_NULL(ls->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_ls_callback, ls->exec);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_ls_service, ls->process);
        TEST_ASSERT_NULL(ls->get_data);
        TEST_ASSERT_NULL(ls->set_data);
        ls->exec(&ush, ls, 0, NULL);
        ls->process(&ush, ls);
}

void test_g_ush_buildin_commands_cd(void)
{
        struct ush_file_descriptor const *cd = &g_ush_buildin_commands[2];

        TEST_ASSERT_EQUAL_STRING("cd", cd->name);
        TEST_ASSERT_NOT_NULL(cd->description);
        TEST_ASSERT_NOT_NULL(cd->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_cd_callback, cd->exec);
        TEST_ASSERT_NULL(cd->process);
        TEST_ASSERT_NULL(cd->get_data);
        TEST_ASSERT_NULL(cd->set_data);
        cd->exec(&ush, cd, 0, NULL);
}

void test_g_ush_buildin_commands_pwd(void)
{
        struct ush_file_descriptor const *pwd = &g_ush_buildin_commands[3];

        TEST_ASSERT_EQUAL_STRING("pwd", pwd->name);
        TEST_ASSERT_NOT_NULL(pwd->description);
        TEST_ASSERT_NOT_NULL(pwd->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_pwd_callback, pwd->exec);
        TEST_ASSERT_NULL(pwd->process);
        TEST_ASSERT_NULL(pwd->get_data);
        TEST_ASSERT_NULL(pwd->set_data);
        pwd->exec(&ush, pwd, 0, NULL);
}

void test_g_ush_buildin_commands_cat(void)
{
        struct ush_file_descriptor const *cat = &g_ush_buildin_commands[4];

        TEST_ASSERT_EQUAL_STRING("cat", cat->name);
        TEST_ASSERT_NOT_NULL(cat->description);
        TEST_ASSERT_NOT_NULL(cat->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_cat_callback, cat->exec);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_cat_service, cat->process);
        TEST_ASSERT_NULL(cat->get_data);
        TEST_ASSERT_NULL(cat->set_data);
        cat->exec(&ush, cat, 0, NULL);
        cat->process(&ush, cat);
}

void test_g_ush_buildin_commands_xxd(void)
{
        struct ush_file_descriptor const *xxd = &g_ush_buildin_commands[5];

        TEST_ASSERT_EQUAL_STRING("xxd", xxd->name);
        TEST_ASSERT_NOT_NULL(xxd->description);
        TEST_ASSERT_NOT_NULL(xxd->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_xxd_callback, xxd->exec);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_xxd_service, xxd->process);
        TEST_ASSERT_NULL(xxd->get_data);
        TEST_ASSERT_NULL(xxd->set_data);
        xxd->exec(&ush, xxd, 0, NULL);
        xxd->process(&ush, xxd);
}

void test_g_ush_buildin_commands_echo(void)
{
        struct ush_file_descriptor const *echo = &g_ush_buildin_commands[6];

        TEST_ASSERT_EQUAL_STRING("echo", echo->name);
        TEST_ASSERT_NOT_NULL(echo->description);
        TEST_ASSERT_NOT_NULL(echo->help);
        TEST_ASSERT_EQUAL(ush_buildin_cmd_echo_callback, echo->exec);
        TEST_ASSERT_NULL(echo->process);
        TEST_ASSERT_NULL(echo->get_data);
        TEST_ASSERT_NULL(echo->set_data);
        echo->exec(&ush, echo, 0, NULL);
}

void test_g_ush_buildin_commands_num(void)
{
        TEST_ASSERT_EQUAL(7, g_ush_buildin_commands_num);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_g_ush_buildin_commands_help);
        RUN_TEST(test_g_ush_buildin_commands_ls);
        RUN_TEST(test_g_ush_buildin_commands_cd);
        RUN_TEST(test_g_ush_buildin_commands_pwd);
        RUN_TEST(test_g_ush_buildin_commands_cat);
        RUN_TEST(test_g_ush_buildin_commands_xxd);
        RUN_TEST(test_g_ush_buildin_commands_echo);
        RUN_TEST(test_g_ush_buildin_commands_num);

        return UNITY_END();
}
