#ifndef USH_SHELL_H
#define USH_SHELL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ush_config.h"

#if USH_CONFIG_ENABLE_FEATURE_SHELL_STYLES == 1

#define USH_SHELL_FONT_STYLE_RESET             "\x1B[0m"
#define USH_SHELL_FONT_STYLE_BOLD              "\x1B[1m"
#define USH_SHELL_FONT_STYLE_DISABLED          "\x1B[2m"
#define USH_SHELL_FONT_STYLE_ITALIC            "\x1B[3m"
#define USH_SHELL_FONT_STYLE_UNDERSCORE        "\x1B[4m"

#define USH_SHELL_FONT_COLOR_RED               "\x1B[31m"
#define USH_SHELL_FONT_COLOR_GREEN             "\x1B[32m"
#define USH_SHELL_FONT_COLOR_YELLOW            "\x1B[33m"
#define USH_SHELL_FONT_COLOR_BLUE              "\x1B[34m"
#define USH_SHELL_FONT_COLOR_MAGENTA           "\x1B[35m"
#define USH_SHELL_FONT_COLOR_CYAN              "\x1B[36m"
#define USH_SHELL_FONT_COLOR_WHITE             "\x1B[37m"

#else

#define USH_SHELL_FONT_STYLE_RESET
#define USH_SHELL_FONT_STYLE_BOLD
#define USH_SHELL_FONT_STYLE_DISABLED
#define USH_SHELL_FONT_STYLE_ITALIC
#define USH_SHELL_FONT_STYLE_UNDERSCORE

#define USH_SHELL_FONT_COLOR_RED
#define USH_SHELL_FONT_COLOR_GREEN
#define USH_SHELL_FONT_COLOR_YELLOW
#define USH_SHELL_FONT_COLOR_BLUE
#define USH_SHELL_FONT_COLOR_MAGENTA
#define USH_SHELL_FONT_COLOR_CYAN
#define USH_SHELL_FONT_COLOR_WHITE

#endif /* USH_CONFIG_ENABLE_FEATURE_SHELL_STYLES */

#ifdef __cplusplus
}
#endif

#endif /* USH_SHELL_H */
