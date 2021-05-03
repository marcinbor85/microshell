file(
    GLOB
    SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_read.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_read_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_read_char.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_parse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_parse_char.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_parse_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_write.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_write_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_prompt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_reset.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_file.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_node.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_node_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_node_mount.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_commands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_autocomp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_autocomp_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush_autocomp_state.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_cd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_help.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_ls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_pwd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_cat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_xxd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands/ush_cmd_echo.c
)

set(
    USH_SRC_FILES
    ${SRC_FILES}
    PARENT_SCOPE
)

set(
    USH_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PARENT_SCOPE
)

set(
    USH_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)

function( build_ush TARGET )
    add_library(
        ${TARGET}
        STATIC
        ${USH_SRC_FILES}
    )

    target_include_directories(
        ${TARGET}
        PUBLIC
        ${ARGV}
        ${USH_INC_DIRS}
    )

    target_compile_options(
        ${TARGET}
        PRIVATE
        -Werror -Wall -Wextra  -g -O0
    )
endfunction()
