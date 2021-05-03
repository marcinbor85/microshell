
create_unit_test( test_unit_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_utils.c
    ${USH_SRC_DIR}/src/ush_utils.c
)

create_unit_test( test_unit_reset
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_reset.c
    ${USH_SRC_DIR}/src/ush_reset.c
)

create_unit_test( test_unit_prompt
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_prompt.c
    ${USH_SRC_DIR}/src/ush_prompt.c
)

create_unit_test( test_unit_write
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_write.c
    ${USH_SRC_DIR}/src/ush_write.c
)

create_unit_test( test_unit_write_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_write_utils.c
    ${USH_SRC_DIR}/src/ush_write_utils.c
)

create_unit_test( test_unit_read
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_read.c
    ${USH_SRC_DIR}/src/ush_read.c
)

create_unit_test( test_unit_read_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_read_utils.c
    ${USH_SRC_DIR}/src/ush_read_utils.c
)

create_unit_test( test_unit_read_char
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_read_char.c
    ${USH_SRC_DIR}/src/ush_read_char.c
)

create_unit_test( test_unit_process
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_process.c
    ${USH_SRC_DIR}/src/ush_process.c
)

create_unit_test( test_unit_parse
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_parse.c
    ${USH_SRC_DIR}/src/ush_parse.c
)

create_unit_test( test_unit_parse_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_parse_utils.c
    ${USH_SRC_DIR}/src/ush_parse_utils.c
)

create_unit_test( test_unit_parse_char
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_parse_char.c
    ${USH_SRC_DIR}/src/ush_parse_char.c
)

create_unit_test( test_unit_commands
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_commands.c
    ${USH_SRC_DIR}/src/ush_commands.c
)

create_unit_test( test_unit_cmd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd.c
    ${USH_SRC_DIR}/src/commands/ush_cmd.c
)

create_unit_test( test_unit_cmd_cd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_cd.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_cd.c
)

create_unit_test( test_unit_cmd_pwd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_pwd.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_pwd.c
)

create_unit_test( test_unit_cmd_echo
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_echo.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_echo.c
    ${USH_SRC_DIR}/src/ush_utils.c
)

create_unit_test( test_unit_cmd_cat
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_cat.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_cat.c
)

create_unit_test( test_unit_cmd_help
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_help.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_help.c
)

create_unit_test( test_unit_cmd_ls
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_ls.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_ls.c
    ${USH_SRC_DIR}/src/ush_utils.c
)

create_unit_test( test_unit_cmd_xxd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_cmd_xxd.c
    ${USH_SRC_DIR}/src/commands/ush_cmd_xxd.c
    ${USH_SRC_DIR}/src/ush_utils.c
)

create_unit_test( test_unit_file
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_file.c
    ${USH_SRC_DIR}/src/ush_file.c
)

create_unit_test( test_unit_ush
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_ush.c
    ${USH_SRC_DIR}/src/ush.c
)

create_unit_test( test_unit_node_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_node_utils.c
    ${USH_SRC_DIR}/src/ush_node_utils.c
)

create_unit_test( test_unit_node_mount
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_node_mount.c
    ${USH_SRC_DIR}/src/ush_node_mount.c
)

create_unit_test( test_unit_node
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_node.c
    ${USH_SRC_DIR}/src/ush_node.c
    ${USH_SRC_DIR}/src/ush_utils.c
)

create_unit_test( test_unit_autocomp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_autocomp.c
    ${USH_SRC_DIR}/src/ush_autocomp.c
)

create_unit_test( test_unit_autocomp_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_autocomp_utils.c
    ${USH_SRC_DIR}/src/ush_autocomp_utils.c
    ${USH_SRC_DIR}/src/ush_utils.c
)

create_unit_test( test_unit_autocomp_state
    ${CMAKE_CURRENT_SOURCE_DIR}/test_unit_autocomp_state.c
    ${USH_SRC_DIR}/src/ush_autocomp_state.c
    ${USH_SRC_DIR}/src/ush_utils.c
)
