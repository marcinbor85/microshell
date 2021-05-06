/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef USH_CONFIG_H
#define USH_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

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
#define USH_CONFIG_TRANSLATION_FILE_NOT_WRITABLE                "file not writable"
#define USH_CONFIG_TRANSLATION_FILE_NOT_READABLE                "file not readable"
#define USH_CONFIG_TRANSLATION_NO_HELP_AVAILABLE                "no help available"
#define USH_CONFIG_TRANSLATION_FILE_NOT_FOUND                   "file not found"
#define USH_CONFIG_TRANSLATION_READ_ONLY_FILE                   "read only file"

#define USH_ASSERT(cond) { }

#ifdef __cplusplus
}
#endif

#endif /* USH_CONFIG_H */
