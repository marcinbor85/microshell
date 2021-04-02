
set( TEST_COMMON_TARGET test_common )
set( UNITY_TARGET unity )

include( ${CMAKE_CURRENT_SOURCE_DIR}/unity.cmake )

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/common )

function(create_test TARGET)
    add_executable( ${TARGET} ${ARGV} )
    target_link_libraries( ${TARGET} PUBLIC ${TEST_COMMON_TARGET})
    add_test( NAME ${TARGET} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} )
endfunction()

create_test( test_init test_init.c )
