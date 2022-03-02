project(
  'microshell',
  'c',
  version: '0.1.0'
)

project_description = 'Lightweight pure C implementation of virtual shell, compatible with VT100 terminal.'

project_headers = [
  'microshell.h',
  'inc/ush.h',
  'inc/ush_assert.h',
  'inc/ush_commands.h',
  'inc/ush_const.h',
  'inc/ush_file.h',
  'inc/ush_internal.h',
  'inc/ush_node.h',
  'inc/ush_shell.h',
  'inc/ush_types.h',
  'inc/ush_utils.h',
]

project_source_files = [
  'src/src/ush.c',
  'src/src/ush_assert.c',
  'src/src/ush_read.c',
  'src/src/ush_read_utils.c',
  'src/src/ush_read_char.c',
  'src/src/ush_parse.c',
  'src/src/ush_parse_char.c',
  'src/src/ush_parse_utils.c',
  'src/src/ush_write.c',
  'src/src/ush_write_utils.c',
  'src/src/ush_prompt.c',
  'src/src/ush_reset.c',
  'src/src/ush_file.c',
  'src/src/ush_node.c',
  'src/src/ush_node_utils.c',
  'src/src/ush_node_mount.c',
  'src/src/ush_utils.c',
  'src/src/ush_commands.c',
  'src/src/ush_process.c',
  'src/src/ush_autocomp.c',
  'src/src/ush_autocomp_utils.c',
  'src/src/ush_autocomp_state.c',
  'src/src/commands/ush_cmd.c',
  'src/src/commands/ush_cmd_cd.c',
  'src/src/commands/ush_cmd_help.c',
  'src/src/commands/ush_cmd_ls.c',
  'src/src/commands/ush_cmd_pwd.c',
  'src/src/commands/ush_cmd_cat.c',
  'src/src/commands/ush_cmd_xxd.c',
  'src/src/commands/ush_cmd_echo.c'
]

project_dependencies = [
]

include_directories = [
]

public_headers = include_directories('src')

build_args = [
  '-Werror',
  '-Wall',
  '-Wextra',
  '-pedantic',
]

link_args = [
]

settings_map = {
  'filename-align-space':        '-DUSH_CONFIG_FILENAME_ALIGN_SPACE',
  'cmd-xxd-columns':             '-DUSH_CONFIG_CMD_XXD_COLUMNS',
}

feature_map = {
  'enable-command-cat':          '-DUSH_CONFIG_ENABLE_COMMAND_CAT',
  'enable-command-cd':           '-DUSH_CONFIG_ENABLE_COMMAND_CD',
  'enable-command-echo':         '-DUSH_CONFIG_ENABLE_COMMAND_ECHO',
  'enable-command-help':         '-DUSH_CONFIG_ENABLE_COMMAND_HELP',
  'enable-command-pwd':          '-DUSH_CONFIG_ENABLE_COMMAND_PWD',
  'enable-command-xxd':          '-DUSH_CONFIG_ENABLE_COMMAND_XXD',
  'enable-command-ls':           '-DUSH_CONFIG_ENABLE_COMMAND_LS',

  'enable-feature-commands':     '-DUSH_CONFIG_ENABLE_FEATURE_COMMANDS',
  'enable-feature-autocomplete': '-DUSH_CONFIG_ENABLE_FEATURE_AUTOCOMPLETE',
  'enable-feature-shell-styles': '-DUSH_CONFIG_ENABLE_FEATURE_SHELL_STYLES',
}

translation_map = {
  'translation-ok':                        '-DUSH_CONFIG_TRANSLATION_OK',
  'translation-error':                     '-DUSH_CONFIG_TRANSLATION_ERROR',
  'translation-directory-not-found':       '-DUSH_CONFIG_TRANSLATION_DIRECTORY_NOT_FOUND',
  'translation-nested-directories-exist':  '-DUSH_CONFIG_TRANSLATION_NESTED_DIRECTORIES_EXIST',
  'translation-cannot-find-parent-node':   '-DUSH_CONFIG_TRANSLATION_CANNOT_FIND_PARENT_NODE',
  'translation-directory-already-mounted': '-DUSH_CONFIG_TRANSLATION_DIRECTORY_ALREADY_MOUNTED',
  'translation-syntax-error':              '-DUSH_CONFIG_TRANSLATION_SYNTAX_ERROR',
  'translation-wrong-arguments':           '-DUSH_CONFIG_TRANSLATION_WRONG_ARGUMENTS',
  'translation-file-not-executable':       '-DUSH_CONFIG_TRANSLATION_FILE_NOT_EXECUTABLE',
  'translation-file-not-writable':         '-DUSH_CONFIG_TRANSLATION_FILE_NOT_WRITABLE',
  'translation-file-not-readable':         '-DUSH_CONFIG_TRANSLATION_FILE_NOT_READABLE',
  'translation-no-help-available':         '-DUSH_CONFIG_TRANSLATION_NO_HELP_AVAILABLE',
  'translation-file-not-found':            '-DUSH_CONFIG_TRANSLATION_FILE_NOT_FOUND',
  'translation-read-only-file':            '-DUSH_CONFIG_TRANSLATION_READ_ONLY_FILE',
}

foreach option_key, option_value : settings_map
  add_project_arguments(option_value + '=' + get_option(option_key).to_string(), language: 'c')
  summary(option_key, get_option(option_key), section: 'Enabled Settingss')
endforeach

foreach option_key, option_value : feature_map
  if get_option(option_key)
    add_project_arguments(option_value, language: 'c')
  endif
  summary(option_key, get_option(option_key), section: 'Enabled Features', bool_yn: true)
endforeach

foreach option_key, option_value : translation_map
  add_project_arguments(option_value + '="' + get_option(option_key) + '"', language: 'c')
endforeach

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language: 'c')
  add_project_arguments('-ggdb3', language: 'c')
endif

# Target

include_directories += public_headers

project_target = static_library(
  meson.project_name(),
  project_source_files,
  install: true,
  c_args: build_args,
  link_args: link_args,
  include_directories: include_directories,
  dependencies: project_dependencies,
)

# Project

# Make this library usable as a Meson subproject
project_dep = declare_dependency(
  include_directories: public_headers,
  link_with: project_target,
)

set_variable('lib' + meson.project_name() + '_dep', project_dep)
