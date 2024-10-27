
cmake_minimum_required(VERSION 3.20)

project(mmpp_thrd_unittest)

set(MMUT_PARENT_HEADER_DIR   CACHE PATH "")
set(MMUT_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMUT_PARENT_LINKER_FILE  CACHE FILEPATH "")

file(GLOB SRC_FILES "*.cpp")

set(MMUT_EXEC_NAME ${PROJECT_NAME}_cxx17)
add_executable(${MMUT_EXEC_NAME} ${SRC_FILES})

target_include_directories(${MMUT_EXEC_NAME} PRIVATE 
  ${MMUT_PARENT_HEADER_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/catch2/include
)

target_compile_features(${MMUT_EXEC_NAME} PRIVATE cxx_std_17)

if (NOT WIN32)
  find_package(Threads REQUIRED)
  target_link_libraries(${MMUT_EXEC_NAME} PRIVATE Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()

target_link_libraries(${MMUT_EXEC_NAME} PRIVATE 
  ${MMUT_PARENT_LINKER_FILE}
)

set_target_properties(${MMUT_EXEC_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN:\$ORIGIN/../lib"
)

add_custom_command(
	TARGET ${MMUT_EXEC_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${MMUT_PARENT_LIBRARY_FILE}
	$<TARGET_FILE_DIR:${MMUT_EXEC_NAME}>
)
