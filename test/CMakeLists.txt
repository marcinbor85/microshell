
set( UNITY_TARGET unity )
include( ${CMAKE_CURRENT_SOURCE_DIR}/unity.cmake )

function(create_test TARGET)
    add_executable( ${TARGET} ${ARGV} )
    target_include_directories( ${TARGET} PUBLIC
        ${PROJECT_SOURCE_DIR}/test/inc
        ${PROJECT_SOURCE_DIR}/ush/inc
    )
    target_link_libraries( ${TARGET} PUBLIC ${UNITY_TARGET})
    add_test( NAME ${TARGET} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} )
endfunction()

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/unit_tests )

add_custom_target(
    check
    COMMAND ctest
    COMMAND gcovr -r ${PROJECT_SOURCE_DIR} --print-summary --html -o test_coverage.html
)
