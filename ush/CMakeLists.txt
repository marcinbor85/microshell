file(
    GLOB
    SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ush.c
)

add_library(
    ${USH_TARGET}
    STATIC
    ${SRC_FILES}
)

target_include_directories(
    ${USH_TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/port/${USH_PORT}
)
