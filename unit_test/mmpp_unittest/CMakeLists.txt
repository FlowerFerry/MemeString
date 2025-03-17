
cmake_minimum_required(VERSION 3.20)

project(mmpp_unittest)

set(MMUT_PARENT_HEADER_DIR   CACHE PATH "")
set(MMUT_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMUT_PARENT_LINKER_FILE  CACHE FILEPATH "")

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
FetchContent_GetProperties(Catch2)
if (NOT catch2_POPULATED)
	FetchContent_Populate(Catch2)
endif()

set(SRC_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/mmpp_unittest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_compare.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_frontback.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_split.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_mid.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_variable_buffer_resize.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_repeat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_concat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_utf8.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_replace.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_string_trim.cpp
)

function(mmut_create_exec_for_stdcxx  exec_name cxx_standard)

  add_executable(${exec_name} ${ARGN})
  
  target_compile_features(${exec_name} PRIVATE ${cxx_standard})

  target_include_directories(${exec_name} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
	${MMUT_PARENT_HEADER_DIR}
	${catch2_SOURCE_DIR}/single_include
  )

  if (NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(${exec_name} PRIVATE Threads::Threads ${CMAKE_DL_LIBS} rt)
  endif()
  
  target_link_libraries(${exec_name} PRIVATE
	${MMUT_PARENT_LINKER_FILE}
  )
  
  set_target_properties(${exec_name} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN"
  )

endfunction()

set(MMUT_CXX11_EXEC_NAME ${PROJECT_NAME}_cxx11)
set(MMUT_CXX14_EXEC_NAME ${PROJECT_NAME}_cxx14)
set(MMUT_CXX17_EXEC_NAME ${PROJECT_NAME}_cxx17)

mmut_create_exec_for_stdcxx(${MMUT_CXX11_EXEC_NAME} cxx_std_11 ${SRC_FILES})
mmut_create_exec_for_stdcxx(${MMUT_CXX14_EXEC_NAME} cxx_std_14 ${SRC_FILES})
mmut_create_exec_for_stdcxx(${MMUT_CXX17_EXEC_NAME} cxx_std_17 ${SRC_FILES})

if(NOT MMUT_PARENT_LIBRARY_FILE STREQUAL "")
  add_custom_command(
	TARGET ${MMUT_CXX11_EXEC_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${MMUT_PARENT_LIBRARY_FILE}
	$<TARGET_FILE_DIR:${MMUT_CXX11_EXEC_NAME}>
  )
endif()

enable_testing()

foreach(cxx_std IN ITEMS 11 14 17)
  add_test(NAME ${PROJECT_NAME}_cxx${cxx_std} COMMAND ${PROJECT_NAME}_cxx${cxx_std})
	
endforeach()
