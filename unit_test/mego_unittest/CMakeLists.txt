
cmake_minimum_required(VERSION 3.20)

project(mego_unittest)

set(MGUT_GIT_HOST "github.com" CACHE STRING "")

set(MGUT_MINUNIT_LIB_URL  "https://${MGUT_GIT_HOST}/siu/minunit.git" CACHE STRING "")
set(MGUT_MINUNIT_LIB_NAME "minunit" CACHE STRING "")


set(FETCHCONTENT_TIMEOUT 120)
set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED_MINUNIT ON)
FetchContent_Declare(
	minunit
	GIT_REPOSITORY ${MGUT_MINUNIT_LIB_URL}
	GIT_TAG master
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/${MGUT_MINUNIT_LIB_NAME}
	GIT_PROGRESS TRUE
)
FetchContent_GetProperties(minunit)
if (NOT minunit_POPULATED)
	FetchContent_Populate(minunit)
endif()

set(SRC_FILES 
    "main.c"
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
  ${minunit_SOURCE_DIR}
)

target_compile_features(
  ${PROJECT_NAME} PRIVATE c_std_11)
  