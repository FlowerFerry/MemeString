
cmake_minimum_required(VERSION 3.20)

project(mego_statvfs_print)

set(MMTEST_PARENT_HEADER_DIR   CACHE PATH "")
set(MMTEST_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMTEST_PARENT_LINKER_FILE  CACHE FILEPATH "")

set(SRC_FILES 
    "main.c"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${MMTEST_PARENT_HEADER_DIR}
)

target_compile_features(
    ${PROJECT_NAME} PRIVATE c_std_11)

    target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${MMTEST_PARENT_LINKER_FILE}
  )
  
  set_target_properties(${PROJECT_NAME} PROPERTIES
      BUILD_WITH_INSTALL_RPATH TRUE
      INSTALL_RPATH "\$ORIGIN:\$ORIGIN/../lib"
  )
  
  if(NOT MMTEST_PARENT_LIBRARY_FILE STREQUAL "")
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${MMTEST_PARENT_LIBRARY_FILE}
      $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
  endif()
  
