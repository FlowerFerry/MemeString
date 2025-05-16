
cmake_minimum_required(VERSION 3.20)

project(meme_atomic_unittest)

set(MMUT_PARENT_HEADER_DIR   CACHE PATH "")
set(MMUT_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMUT_PARENT_LINKER_FILE  CACHE FILEPATH "")

set(MMUT_GIT_HOST "github.com" CACHE STRING "")

set(MMUT_MINUNIT_REPO "siu/minunit" CACHE STRING "")
set(MMUT_MINUNIT_TAG "master" CACHE STRING "")

set(FETCHCONTENT_TIMEOUT 120)
set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED_MINUNIT ON)
FetchContent_Declare(
	minunit
	GIT_REPOSITORY https://${MMUT_GIT_HOST}/${MMUT_MINUNIT_REPO}.git
	GIT_TAG ${MMUT_MINUNIT_TAG}
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/minunit
	GIT_PROGRESS TRUE
)
FetchContent_GetProperties(minunit)
if (NOT minunit_POPULATED)
	FetchContent_Populate(minunit)
endif()

set(SRC_FILES
	"main.c"
	
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${MMUT_PARENT_HEADER_DIR}
  ${minunit_SOURCE_DIR}
)

if (NOT WIN32)
  find_package(Threads REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${MMUT_PARENT_LINKER_FILE}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN:\$ORIGIN/../lib"
)

if(NOT MMUT_PARENT_LIBRARY_FILE STREQUAL "")
  add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${MMUT_PARENT_LIBRARY_FILE}
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()
