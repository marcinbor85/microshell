
set( EXAMPLE_DEMO_TARGET demo )

set ( USH_TARGET ush_demo )
build_ush ( ${USH_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(
    ${USH_TARGET}
    PUBLIC
    USH_CONFIG_PLATFORM_POSIX
)

add_executable(
    ${EXAMPLE_DEMO_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/demo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface.c
    ${CMAKE_CURRENT_SOURCE_DIR}/commands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/terminal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/shell.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/fs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/path/root.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/path/etc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/path/dev.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fs/path/bin.c
)
target_link_libraries( ${EXAMPLE_DEMO_TARGET} PUBLIC ${USH_TARGET})
target_compile_options( ${EXAMPLE_DEMO_TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
