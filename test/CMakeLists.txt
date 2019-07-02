cmake_minimum_required(VERSION 3.9)
project(MCMF)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIR_SRCS)
link_directories("${PROJECT_SOURCE_DIR}/../lib")
add_executable(testMCMF ${DIR_SRCS})

# 添加链接库
target_link_libraries(testMCMF mcmf glpk)