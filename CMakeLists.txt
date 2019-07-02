cmake_minimum_required(VERSION 3.9)
project(MCMF)

set(CMAKE_CXX_STANDARD 11)

#add_subdirectory(include)
add_subdirectory(src)



# set the project name
PROJECT(MCMF_WORKSPACE)


# directive to print detail build log
SET(CMAKE_VERBOSE_MAKEFILE ON)
# set the install prefix for make install
SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/out CACHE PATH "" FORCE)
# set the output directory for bin during building(make)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tmp/bin)
# set the output directory for libs during building(make)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tmp/lib)


## include and build  submodule build configuration
#ADD_SUBDIRECTORY(src)

# include and build the test submodule
ADD_SUBDIRECTORY(test)

# add test target
ENABLE_TESTING()
ADD_TEST(UTTest ${PROJECT_BINARY_DIR}/tmp/bin/TestCase)

# install directory to target directory ${CMAKE_INSTALL_PREFIX}/.
INSTALL(DIRECTORY docs DESTINATION .)
# install files to target directory ${CMAKE_INSTALL_PREFIX}/.
INSTALL(FILES COPYRIGHT DESTINATION .)

# current directory of CMakeLists.txt
MESSAGE("test CMAKE_CURRENT_SOURCE_DIR " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("test CMAKE_CURRRENT_BINARY_DIR " ${CMAKE_CURRRENT_BINARY_DIR})
MESSAGE("test PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
MESSAGE("test PROJECT_SOURCE_DIR " ${PROJECT_BINARY_DIR})
MESSAGE("test CMAKE_INSTALL_PREFIX " ${CMAKE_INSTALL_PREFIX})

# environment variable
MESSAGE("test CMAKE_INCLUDE_PATH " $ENV{CMAKE_INCLUDE_PATH})
MESSAGE("test CMAKE_LIBRARY_PATH " $ENV{CMAKE_LIBRARY_PATH})
