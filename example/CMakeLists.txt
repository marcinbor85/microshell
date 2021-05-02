
set( EXAMPLE_BASIC_TARGET example_basic )

set( USH_TARGET ush )

add_library(
    ${USH_TARGET}
    STATIC
    ${USH_SRC_FILES}
)

target_include_directories(
    ${USH_TARGET}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/ush/inc
    ${PROJECT_SOURCE_DIR}/ush/port/posix
)

add_executable( ${EXAMPLE_BASIC_TARGET} basic.c )
target_link_libraries( ${EXAMPLE_BASIC_TARGET} PUBLIC ${USH_TARGET})
target_compile_options( ${EXAMPLE_BASIC_TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
