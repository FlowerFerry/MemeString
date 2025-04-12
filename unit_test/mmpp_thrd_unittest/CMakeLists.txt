
cmake_minimum_required(VERSION 3.20)

project(mmpp_thrd_unittest)

set(MMUT_PARENT_HEADER_DIR   CACHE PATH "")
set(MMUT_PARENT_LIBRARY_FILE CACHE FILEPATH "")
set(MMUT_PARENT_LINKER_FILE  CACHE FILEPATH "")

set(MMUT_GIT_HOST "github.com" CACHE STRING "")

function(mmut_create_exec_for_stdcxx  exec_name cxx_standard)

  add_executable(${exec_name} ${ARGN})
  
  target_compile_features(${exec_name} PRIVATE ${cxx_standard})

  target_include_directories(${exec_name} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
	${catch2_SOURCE_DIR}/single_include
	${MMUT_PARENT_HEADER_DIR}
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

set(SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

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

