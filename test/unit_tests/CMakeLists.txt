
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
