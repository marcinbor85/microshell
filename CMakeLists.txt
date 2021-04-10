cmake_minimum_required( VERSION 3.0 )

project( ush LANGUAGES C VERSION 0.1.0 )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( USH_TARGET ush )
set( USH_PORT posix )

enable_testing( )

add_compile_options( -Werror -Wall -Wextra -pedantic -g -O0)

add_subdirectory( ${PROJECT_SOURCE_DIR}/test )
add_subdirectory( ${PROJECT_SOURCE_DIR}/example )
add_subdirectory( ${PROJECT_SOURCE_DIR}/ush )
