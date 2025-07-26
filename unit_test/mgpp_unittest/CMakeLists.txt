
cmake_minimum_required(VERSION 3.20)

project(mgpp_unittest)

option(MGUT_INTERNAL_FETCHCONTENT "Use FetchContent to download internal dependencies" ON)

if (MGUT_INTERNAL_FETCHCONTENT)
  set(MGUT_GIT_HOST "github.com" CACHE STRING "")
  set(MGUT_CATCH2_REPO "catchorg/Catch2" CACHE STRING "")
  set(MGUT_CATCH2_TAG "v2.13.9" CACHE STRING "")
  set(MGUT_CATCH2_SOURCE_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/catch2-${MGUT_CATCH2_TAG}" CACHE STRING "")

  set(FETCHCONTENT_TIMEOUT 120)
  set(FETCHCONTENT_QUIET FALSE)

  include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://${MGUT_GIT_HOST}/${MGUT_CATCH2_REPO}.git
    GIT_TAG ${MGUT_CATCH2_TAG}
    SOURCE_DIR ${MGUT_CATCH2_SOURCE_LOCAL_DIR}
    GIT_PROGRESS TRUE
  )
  FetchContent_GetProperties(catch2)
  if (NOT catch2_POPULATED)
    FetchContent_MakeAvailable(catch2)
  endif()
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
