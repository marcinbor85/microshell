
create_test( test_unit_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_utils.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_utils.c
)

create_test( test_unit_reset
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_reset.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_reset.c
)

create_test( test_unit_prompt
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_prompt.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_prompt.c
)

create_test( test_unit_write
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_write.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_write.c
)

create_test( test_unit_write_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_write_utils.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_write_utils.c
)

create_test( test_unit_read
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_read.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_read.c
)

create_test( test_unit_read_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_read_utils.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_read_utils.c
)

create_test( test_unit_read_char
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_read_char.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_read_char.c
)

create_test( test_unit_process
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_process.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_process.c
)

create_test( test_unit_parse
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_parse.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_parse.c
)

create_test( test_unit_parse_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_parse_utils.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_parse_utils.c
)

create_test( test_unit_parse_char
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_parse_char.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_parse_char.c
)

create_test( test_unit_commands
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_commands.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_commands.c
)

create_test( test_unit_cmd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd.c
    ${PROJECT_SOURCE_DIR}/ush/src/commands/ush_cmd.c
)

create_test( test_unit_cmd_cd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_cd.c
    ${PROJECT_SOURCE_DIR}/ush/src/commands/ush_cmd_cd.c
)

create_test( test_unit_cmd_pwd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_pwd.c
    ${PROJECT_SOURCE_DIR}/ush/src/commands/ush_cmd_pwd.c
)

create_test( test_unit_cmd_echo
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_echo.c
    ${PROJECT_SOURCE_DIR}/ush/src/commands/ush_cmd_echo.c
)

create_test( test_unit_cmd_cat
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_cat.c
    ${PROJECT_SOURCE_DIR}/ush/src/commands/ush_cmd_cat.c
)

create_test( test_unit_file
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_file.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_file.c
)

create_test( test_unit_ush
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_ush.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush.c
)

create_test( test_unit_node_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_node_utils.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_node_utils.c
)

create_test( test_unit_node_mount
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_node_mount.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_node_mount.c
)

create_test( test_unit_node
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_node.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_node.c
    ${PROJECT_SOURCE_DIR}/ush/src/ush_utils.c
)
