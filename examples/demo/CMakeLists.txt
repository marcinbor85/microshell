
set( EXAMPLE_DEMO_TARGET demo )

set ( USH_TARGET ush_demo )
build_ush ( ${USH_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable( ${EXAMPLE_DEMO_TARGET} demo.c )
target_link_libraries( ${EXAMPLE_DEMO_TARGET} PUBLIC ${USH_TARGET})
target_compile_options( ${EXAMPLE_DEMO_TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
