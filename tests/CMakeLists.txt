
set( UNITY_TARGET unity )

add_library(
    ${UNITY_TARGET}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src/unity.c
)
target_include_directories(
    ${UNITY_TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src
)

set ( USH_TARGET ush_testfunc )
build_ush ( ${USH_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/func_tests
)

set ( TEST_FUNC testfunc )
add_library(
    ${TEST_FUNC}
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/func_tests/test_func.c
)
target_include_directories(
    ${TEST_FUNC}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/tests/func_tests
)
target_link_libraries(
    ${TEST_FUNC}
    PUBLIC
    ${UNITY_TARGET}
    ${USH_TARGET}
)
target_compile_options(
    ${TEST_FUNC}
    PRIVATE
    -Werror -Wall -Wextra  -g -O0
)

function(create_unit_test TARGET)
    add_executable( ${TARGET} ${ARGV} )
    target_include_directories( ${TARGET} PUBLIC
        ${PROJECT_SOURCE_DIR}/tests/unit_tests
        ${USH_INC_DIRS}
    )
    target_link_libraries( ${TARGET} PUBLIC ${UNITY_TARGET})
    target_compile_options( ${TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
    add_test( NAME ${TARGET} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} )
endfunction()

function(create_func_test TARGET)
    add_executable( ${TARGET} ${ARGV} )
    target_link_libraries( ${TARGET} PUBLIC ${TEST_FUNC} )
    target_compile_options( ${TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
    add_test( NAME ${TARGET} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} )
endfunction()

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/unit_tests )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/func_tests )

add_custom_target(
    coverage
    COMMAND gcovr -r ${PROJECT_SOURCE_DIR} --exclude=${PROJECT_SOURCE_DIR}/tests/Unity/ --exclude=${PROJECT_SOURCE_DIR}/examples/ --print-summary --html --html-details -o test_coverage.html
)
