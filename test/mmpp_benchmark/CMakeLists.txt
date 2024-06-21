
cmake_minimum_required(VERSION 3.20)

project(mmpp_benchmark)

set(MMBM_PARENT_HEADER_DIR   CACHE PATH "")
set(MMBM_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMBM_PARENT_LINKER_FILE  CACHE FILEPATH "")

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

include(FetchContent)

FetchContent_Declare(
	benchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG v1.8.4
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/benchmark-v1.8.4
)

FetchContent_MakeAvailable(benchmark)

file(GLOB SRC_FILES "*.cpp")

add_executable(mmpp_benchmark
	${SRC_FILES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ./
  ${MMBM_PARENT_HEADER_DIR}
)

if (NOT WIN32)
  find_package(Threads REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE 
  benchmark::benchmark
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
