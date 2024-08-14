
cmake_minimum_required(VERSION 3.20)

project(mmpp_thrd_unittest)

set(MMBM_PARENT_HEADER_DIR   CACHE PATH "")
set(MMBM_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMBM_PARENT_LINKER_FILE  CACHE FILEPATH "")

file(GLOB SRC_FILES "*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${MMBM_PARENT_HEADER_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/catch2/include
)

if (NOT WIN32)
  find_package(Threads REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${MMBM_PARENT_LINKER_FILE}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN:\$ORIGIN/../lib"
)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${MMBM_PARENT_LIBRARY_FILE}
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)
