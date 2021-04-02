file(
    GLOB
    UNITY_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src/unity.c
)

add_library(
    ${UNITY_TARGET}
    STATIC
    ${UNITY_SRC_FILES}
)

target_include_directories(
    ${UNITY_TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src
)
