
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
