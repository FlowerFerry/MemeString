
cmake_minimum_required(VERSION 3.20)

project(meme_macro_print)

set(MMMP_PARENT_HEADER_DIR CACHE PATH "")

set(SRC_FILES 
    "main.c"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

if(MMMP_PARENT_HEADER_DIR STREQUAL "")
    set(MMMP_PARENT_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
endif()

target_compile_features(
    ${PROJECT_NAME} PRIVATE c_std_11)

target_include_directories(
    ${PROJECT_NAME} PRIVATE ${MMMP_PARENT_HEADER_DIR})

