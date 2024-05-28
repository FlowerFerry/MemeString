
cmake_minimum_required(VERSION 3.14)

if (WIN32)
  set (MEME_PROJECT_NAME libmeme_string PARENT_SCOPE)
else()
  set (MEME_PROJECT_NAME    meme_string PARENT_SCOPE)
endif()

project(${MEME_PROJECT_NAME} VERSION 0.0.1 LANGUAGES C)

# 设置 C 语言标准
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加 include 路径
# include_directories(3rdparty/cvector/include include include_private)

# 添加链接库
if (NOT WIN32)
  find_package(Threads REQUIRED)
  set(LINK_LIBS Threads::Threads ${CMAKE_DL_LIBS} rt)
endif()

# 设置编译选项
# set(CMAKE_C_VISIBILITY_PRESET hidden)

# 生成 shared 库
file(GLOB SRC_FILES "src/*.c")
add_library(${MEME_PROJECT_NAME} SHARED ${SRC_FILES})
target_compile_definitions(${MEME_PROJECT_NAME} PRIVATE MEME_OPTION__BUILD_SHARED)
target_compile_features(${MEME_PROJECT_NAME} PRIVATE c_std_11)
target_include_directories(${MEME_PROJECT_NAME} PUBLIC include)
target_include_directories(${MEME_PROJECT_NAME} PRIVATE 3rdparty/cvector/include include_private)
target_link_libraries(${MEME_PROJECT_NAME} PRIVATE ${LINK_LIBS})
set_target_properties(${MEME_PROJECT_NAME} PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(${MEME_PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# 设置 rpath
set_target_properties(${MEME_PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN")

# 设置版本号
# string(TIMESTAMP BUILD_DATE "%Y%m%d%H%M")

set_target_properties(${MEME_PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS ${MEME_PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
install(DIRECTORY include/ DESTINATION include)
