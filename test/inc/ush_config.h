
#ifndef USH_CONFIG_H
#define USH_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define USH_CONFIG_PATH_MAX_LENGTH                              128
#define USH_CONFIG_FILENAME_ALIGN_SPACE                         16
#define USH_CONFIG_CMD_XXD_COLUMNS                              8

#define USH_CONFIG_ENABLE_COMMAND_CAT                           1
#define USH_CONFIG_ENABLE_COMMAND_CD                            1
#define USH_CONFIG_ENABLE_COMMAND_ECHO                          1
#define USH_CONFIG_ENABLE_COMMAND_HELP                          1
#define USH_CONFIG_ENABLE_COMMAND_LS                            1
#define USH_CONFIG_ENABLE_COMMAND_PWD                           1
#define USH_CONFIG_ENABLE_COMMAND_XXD                           1

#define USH_CONFIG_ENABLE_FEATURE_COMMANDS                      1
#define USH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE                  1
#define USH_CONFIG_ENABLE_FEATURE_SHELL_STYLES                  1

#define USH_CONFIG_TRANSLATION_OK                               "ok"
#define USH_CONFIG_TRANSLATION_ERROR                            "error"
#define USH_CONFIG_TRANSLATION_DIRECTORY_NOT_FOUND              "directory not found"
#define USH_CONFIG_TRANSLATION_NESTED_DIRECTORIES_EXIST         "nested directories exist"
#define USH_CONFIG_TRANSLATION_CANNOT_FIND_PARENT_NODE          "cannot find parent node"
#define USH_CONFIG_TRANSLATION_DIRECTORY_ALREADY_MOUNTED        "directory already mounted"
#define USH_CONFIG_TRANSLATION_SYNTAX_ERROR                     "syntax error"
#define USH_CONFIG_TRANSLATION_WRONG_ARGUMENTS                  "wrong arguments"
#define USH_CONFIG_TRANSLATION_FILE_NOT_EXECUTABLE              "file not executable"
#define USH_CONFIG_TRANSLATION_NO_HELP_AVAILABLE                "no help available"
#define USH_CONFIG_TRANSLATION_FILE_NOT_FOUND                   "file not found"
#define USH_CONFIG_TRANSLATION_NO_DATA_AVAILABLE                "no data available"
#define USH_CONFIG_TRANSLATION_READ_ONLY_FILE                   "read only file"

#define USH_ASSERT(cond) { }

#ifdef __cplusplus
}
#endif

#endif /* USH_CONFIG_H */
