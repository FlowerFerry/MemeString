cmake_minimum_required(VERSION 3.14)

project(meme_string VERSION 0.0.1 LANGUAGES C)

# 设置 C 语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加 include 路径
include_directories(3rdparty/cvector/include include_private)

# 添加链接库
find_package(Threads REQUIRED)
set(LINK_LIBS Threads::Threads ${CMAKE_DL_LIBS} rt)

# 设置编译选项
set(CMAKE_C_VISIBILITY_PRESET hidden)

# 生成 shared 库
file(GLOB SRC_FILES "src/*.c")
add_library(meme_string SHARED ${SRC_FILES})
target_compile_definitions(meme_string PRIVATE MEME_OPTION__BUILD_SHARED)
target_link_libraries(meme_string PRIVATE ${LINK_LIBS})
target_include_directories(meme_string PUBLIC include)

# 设置 rpath
set_target_properties(meme_string PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN")

# 设置版本号
string(TIMESTAMP BUILD_DATE "%Y%m%d%H%M")
set_target_properties(meme_string PROPERTIES VERSION ${PROJECT_VERSION}.${BUILD_DATE})
