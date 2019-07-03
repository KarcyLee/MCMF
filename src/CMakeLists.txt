cmake_minimum_required(VERSION 3.9)
project(MCMF)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)

# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

link_directories("${PROJECT_SOURCE_DIR}/../lib")

add_library(mcmf SHARED ${DIR_LIB_SRCS})

# 添加链接库
target_link_libraries(mcmf glpk)