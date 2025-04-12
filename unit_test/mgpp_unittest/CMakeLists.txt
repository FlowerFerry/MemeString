
cmake_minimum_required(VERSION 3.20)

project(mgpp_unittest)

set(MMUT_GIT_HOST "github.com" CACHE STRING "")

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
