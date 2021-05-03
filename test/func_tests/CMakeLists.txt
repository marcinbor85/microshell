
create_func_test( test_func_help
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_help.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_pwd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_pwd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_input
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_autocomp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_autocomp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_cd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)
