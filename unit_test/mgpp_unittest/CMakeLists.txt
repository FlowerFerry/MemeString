
cmake_minimum_required(VERSION 3.20)

project(mgpp_unittest)

set(MMUT_GIT_HOST "github.com" CACHE STRING "")

set(FETCHCONTENT_TIMEOUT 120)
set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED_MINUNIT ON)
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://${MMUT_GIT_HOST}/catchorg/Catch2.git
	GIT_TAG v2.13.9
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/Catch2-v2.13.9
	GIT_PROGRESS TRUE
)
FetchContent_GetProperties(catch2)
if (NOT catch2_POPULATED)
	FetchContent_Populate(catch2)
endif()

set(SRC_FILES
	main.cpp
	test_dir.cpp
	test_endian.cpp
	test_path.cpp
	test_type_traits.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${catch2_SOURCE_DIR}/single_include
)

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()
