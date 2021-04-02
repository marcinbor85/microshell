
set( EXAMPLE_BASIC_TARGET example_basic )

add_executable( ${EXAMPLE_BASIC_TARGET} basic.c )
target_link_libraries( ${EXAMPLE_BASIC_TARGET} PUBLIC ${USH_TARGET})
target_compile_options( ${EXAMPLE_BASIC_TARGET} PRIVATE -Werror -Wall -Wextra -pedantic )
