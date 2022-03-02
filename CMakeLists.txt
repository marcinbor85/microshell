cmake_minimum_required( VERSION 3.0 )

project( ush LANGUAGES C VERSION 0.1.0 )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

enable_testing( )

option(ENABLE_COMMAND_CAT  "Enable CAT command" ON)
option(ENABLE_COMMAND_CD   "Enable CD command" ON)
option(ENABLE_COMMAND_ECHO "Enable ECHO command" ON)
option(ENABLE_COMMAND_HELP "Enable HELP command" ON)
option(ENABLE_COMMAND_PWD  "Enable PWD command" ON)
option(ENABLE_COMMAND_XXD  "Enable XXD command" ON)
option(ENABLE_COMMAND_LS   "Enable LS command" ON)

option(ENABLE_FEATURE_COMMANDS     "Enable feature commands" ON)
option(ENABLE_FEATURE_AUTOCOMPLETE "Enable auto-complete" ON)
option(ENABLE_FEATURE_SHELL_STYLES "Enable shell styles" ON)

set(FILENAME_ALIGN_SPACE "16" CACHE STRING "Alignment space for filenames")
set(CMD_XXD_COLUMNS      "16" CACHE STRING "XXD column count")

set(OPTIONS ENABLE_COMMAND_CAT
            ENABLE_COMMAND_CD
            ENABLE_COMMAND_ECHO
            ENABLE_COMMAND_HELP
            ENABLE_COMMAND_PWD
            ENABLE_COMMAND_XXD
            ENABLE_COMMAND_LS)

set(FEATURE_OPTIONS ENABLE_FEATURE_COMMANDS
                    ENABLE_FEATURE_AUTOCOMPLETE
                    ENABLE_FEATURE_SHELL_STYLES)

set(CONFIG_OPTIONS FILENAME_ALIGN_SPACE
                   CMD_XXD_COLUMNS)

set(TRANSLATION_OK                        "ok" CACHE STRING "Translation string")
set(TRANSLATION_ERROR                     "error" CACHE STRING "Translation string")
set(TRANSLATION_DIRECTORY_NOT_FOUND       "directory not found" CACHE STRING "Translation string")
set(TRANSLATION_NESTED_DIRECTORIES_EXIST  "nested directories exist" CACHE STRING "Translation string")
set(TRANSLATION_CANNOT_FIND_PARENT_NODE   "cannot find parent node" CACHE STRING "Translation string")
set(TRANSLATION_DIRECTORY_ALREADY_MOUNTED "directory already mounted" CACHE STRING "Translation string")
set(TRANSLATION_SYNTAX_ERROR              "syntax error" CACHE STRING "Translation string")
set(TRANSLATION_WRONG_ARGUMENTS           "wrong arguments" CACHE STRING "Translation string")
set(TRANSLATION_FILE_NOT_EXECUTABLE       "file not executable" CACHE STRING "Translation string")
set(TRANSLATION_FILE_NOT_WRITABLE         "file not writable" CACHE STRING "Translation string")
set(TRANSLATION_FILE_NOT_READABLE         "file not readable" CACHE STRING "Translation string")
set(TRANSLATION_NO_HELP_AVAILABLE         "no help available" CACHE STRING "Translation string")
set(TRANSLATION_FILE_NOT_FOUND            "file not found" CACHE STRING "Translation string")
set(TRANSLATION_READ_ONLY_FILE            "read only file" CACHE STRING "Translation string")

set(TRANSLATION_OPTIONS TRANSLATION_OK
                        TRANSLATION_ERROR
                        TRANSLATION_DIRECTORY_NOT_FOUND
                        TRANSLATION_NESTED_DIRECTORIES_EXIST
                        TRANSLATION_CANNOT_FIND_PARENT_NODE
                        TRANSLATION_DIRECTORY_ALREADY_MOUNTED
                        TRANSLATION_SYNTAX_ERROR
                        TRANSLATION_WRONG_ARGUMENTS
                        TRANSLATION_FILE_NOT_EXECUTABLE
                        TRANSLATION_FILE_NOT_WRITABLE
                        TRANSLATION_FILE_NOT_READABLE
                        TRANSLATION_NO_HELP_AVAILABLE
                        TRANSLATION_FILE_NOT_FOUND
                        TRANSLATION_READ_ONLY_FILE)

foreach(OPT IN LISTS OPTIONS FEATURE_OPTIONS)
  if (${OPT})
  add_compile_definitions(USH_CONFIG_${OPT})
  message(STATUS ${OPT})
  endif()
endforeach()

foreach(_opt IN LISTS CONFIG_OPTIONS)
  message(STATUS USH_CONFIG_${_opt}=${${_opt}})
endforeach()

foreach(_opt IN LISTS TRANSLATION_OPTIONS)
  message(STATUS USH_CONFIG_${_opt}="${${_opt}}")
endforeach()

foreach(_opt IN LISTS TRANSLATION_OPTIONS)
  add_compile_definitions(USH_CONFIG_${_opt}="${${_opt}}")
endforeach()

add_compile_definitions(USH_CONFIG_FILENAME_ALIGN_SPACE=${FILENAME_ALIGN_SPACE})
add_compile_definitions(USH_CONFIG_CMD_XXD_COLUMNS=${CMD_XXD_COLUMNS})

set (CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -pedantic -g -O0 --coverage" )
set (CMAKE_C_FLAGS "-Werror -Wall -Wextra -pedantic -g -O0 --coverage" )
set (CMAKE_EXE_LINKER_FLAGS "--coverage" )

add_subdirectory( ${PROJECT_SOURCE_DIR}/src )
add_subdirectory( ${PROJECT_SOURCE_DIR}/tests )
add_subdirectory( ${PROJECT_SOURCE_DIR}/examples )
