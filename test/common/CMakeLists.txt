file(
    GLOB
    TEST_COMMON_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_common.c
)

add_library(
    ${TEST_COMMON_TARGET}
    STATIC
    ${TEST_COMMON_SRC_FILES}
)

target_include_directories(
    ${TEST_COMMON_TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( ${TEST_COMMON_TARGET} PUBLIC ${UNITY_TARGET} ${USH_TARGET} )
