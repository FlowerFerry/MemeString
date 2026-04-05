
cmake_minimum_required(VERSION 3.20)

project(mego_time_print)

set(SRC_FILES 
    "main.c"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
)

target_compile_features(
  ${PROJECT_NAME} PRIVATE c_std_11)
  