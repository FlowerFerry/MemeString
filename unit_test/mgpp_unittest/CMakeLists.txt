
cmake_minimum_required(VERSION 3.20)

project(mgpp_unittest)

aux_source_directory(. SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/catch2/include
)

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()
