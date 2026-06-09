
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
	test_byte_swap.cpp
	test_endian_net.cpp
	test_iterator.cpp
	test_objbuf.cpp
	test_trivial_create.cpp
	test_traverse.cpp
	test_snprintf.cpp
	test_atomic_shared_ptr.cpp
	test_cow_ptr.cpp
	test_emtx_cow_ptr.cpp
	test_hashable_weak_ptr.cpp
	test_atomic_counter.cpp
	test_template_random.cpp
	test_template_indexer.cpp
	test_value_picker.cpp
	test_ref_counter.cpp
	test_spin_mutex.cpp
	test_synchronized.cpp
	test_function_wrapper.cpp
	test_c_wrap_shared_ptr.cpp
	test_c_wrap_smart_ptr.cpp
	test_c_wrap_weak_ptr.cpp
	test_scope_locker.cpp
	test_numeric_id.cpp
	test_noncopyable.cpp
	test_null_mutex.cpp
	test_singleton.cpp
	test_scope_cleanup.cpp
	test_endian_types.cpp
	test_numeric_types.cpp
	test_rune.cpp
	test_rune_index.cpp
	test_singleton_weak.cpp
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
