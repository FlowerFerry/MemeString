
cmake_minimum_required(VERSION 3.14)

if (WIN32)
  set (MEME_PROJECT_NAME libmeme_string)
else()
  set (MEME_PROJECT_NAME    meme_string)
endif()

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set (MEMESTRING_PROJECT_NAME ${MEME_PROJECT_NAME} PARENT_SCOPE)
endif()

project(${MEME_PROJECT_NAME} VERSION 0.0.1 LANGUAGES C)

option(MEMESTRING_SONAME_SUFFIX_ENABLE "Enable soname suffix of  of meme library" ON)
option(MEMESTRING_BENCHMARK_ENABLE "Enable benchmark of meme library" OFF)
option(MEMESTRING_UNITTEST_ENABLE  "Enable unit test of meme library"  OFF)

set(MMSTR_GIT_HOST "github.com" CACHE STRING "")

# 设置 C 语言标准
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加 include 路径
# include_directories(3rdparty/cvector/include include include_private)

message(STATUS "MEMESTRING_SONAME_SUFFIX_ENABLE: ${MEMESTRING_SONAME_SUFFIX_ENABLE}")
message(STATUS "MEMESTRING_BENCHMARK_ENABLE: ${MEMESTRING_BENCHMARK_ENABLE}")
message(STATUS "MEMESTRING_UNITTEST_ENABLE: ${MEMESTRING_UNITTEST_ENABLE}")

include(CheckCCompilerFlag)

# 添加链接库
if (NOT WIN32)
  find_package(Threads REQUIRED)
  set(LINK_LIBS Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()

# 设置编译选项
# set(CMAKE_C_VISIBILITY_PRESET hidden)

# 生成 shared 库
set(SRC_FILES
    src/meme_string__basics.c
    src/meme_string__stack.c
    src/meme_string_builder.c
    src/meme_impl__string_large.c
    src/meme_impl__string_medium.c
    src/meme_impl__string_small.c
    src/meme_impl__string_user.c
    src/meme_rune.c
    src/meme_memory.c
    src/meme_impl__algorithm.c
    src/meme_utf_converter.c
    src/meme_utf_u8rune.c
    src/meme_utf_u16rune.c
    src/meme_utf_u32rune.c
    src/meme_variable_buffer.c
    src/meme_buffer.c
    src/meme_variant.c
    src/meme_atomic__basics.c
    src/meme_conc_atomic.c
    src/meme_simd_handler.c
)

add_library(${MEME_PROJECT_NAME} SHARED ${SRC_FILES})
target_compile_definitions(${MEME_PROJECT_NAME} PRIVATE MEME_OPTION__BUILD_SHARED)
target_compile_features(${MEME_PROJECT_NAME} PRIVATE c_std_11)

target_include_directories(${MEME_PROJECT_NAME} PUBLIC include)
target_include_directories(${MEME_PROJECT_NAME} PRIVATE 3rdparty/cvector/include include_private)

target_link_libraries(${MEME_PROJECT_NAME} PRIVATE ${LINK_LIBS})

set_target_properties(${MEME_PROJECT_NAME} PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(${MEME_PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (MSVC)
  target_link_options(${MEME_PROJECT_NAME}  PRIVATE 
      "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/projects/vs/libmeme_string/export.def")
endif()

if (NOT MSVC)
  check_c_compiler_flag("-mavx2" MMSTR_COMPILER_SUPPORTS_AVX2)
  check_c_compiler_flag("-mfpu=neon" MMSTR_COMPILER_SUPPORTS_NEON)

  if (MMSTR_COMPILER_SUPPORTS_AVX2)
	  target_compile_options(${MEME_PROJECT_NAME} PRIVATE -mavx2)
    message(STATUS "Compiler supports AVX2")
  endif()

  if (MMSTR_COMPILER_SUPPORTS_NEON)
    target_compile_options(${MEME_PROJECT_NAME} PRIVATE -mfpu=neon)
    message(STATUS "Compiler supports NEON")
  endif()

endif()

# 设置 rpath
set_target_properties(${MEME_PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN")

# 设置版本号
# string(TIMESTAMP BUILD_DATE "%Y%m%d%H%M")

if (MEMESTRING_SONAME_SUFFIX_ENABLE)
  set_target_properties(${MEME_PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
endif()

install(TARGETS ${MEME_PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
install(DIRECTORY include/ DESTINATION include)

if (MEMESTRING_BENCHMARK_ENABLE)
  set(MMBM_PARENT_HEADER_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "" FORCE)
  set(MMBM_PARENT_LIBRARY_FILE $<TARGET_FILE:${MEME_PROJECT_NAME}> CACHE FILEPATH "" FORCE)
  set(MMBM_PARENT_LINKER_FILE  $<TARGET_LINKER_FILE:${MEME_PROJECT_NAME}> CACHE FILEPATH "" FORCE)
  set(MMBM_GIT_HOST ${MMSTR_GIT_HOST} CACHE STRING "" FORCE)
  add_subdirectory(test/mmpp_benchmark)
endif()

if (MEMESTRING_UNITTEST_ENABLE)  
  set(MMUT_PARENT_HEADER_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "" FORCE)
  set(MMUT_PARENT_LIBRARY_FILE $<TARGET_FILE:${MEME_PROJECT_NAME}> CACHE FILEPATH "" FORCE)
  set(MMUT_PARENT_LINKER_FILE  $<TARGET_LINKER_FILE:${MEME_PROJECT_NAME}> CACHE FILEPATH "" FORCE)
  set(MMUT_GIT_HOST ${MMSTR_GIT_HOST} CACHE STRING "" FORCE)
  add_subdirectory(unit_test/mmpp_unittest)
  add_subdirectory(unit_test/mmpp_thrd_unittest)
  add_subdirectory(unit_test/meme_atomic_unittest)
  add_subdirectory(unit_test/meme_simd_unittest)
endif()

