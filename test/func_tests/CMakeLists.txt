
create_func_test( test_func_cmd_help
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cmd_help.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_cmd_pwd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cmd_pwd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_cmd_cd
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cmd_cd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_cmd_echo
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cmd_echo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_cmd_ls
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cmd_ls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)

create_func_test( test_func_cmd_cat
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_cmd_cat.c
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

create_func_test( test_func_exec
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func_exec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_func.c
)
