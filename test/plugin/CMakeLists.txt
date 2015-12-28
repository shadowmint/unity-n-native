## Project
cmake_minimum_required(VERSION 3.1)
project(plugin)

# Deps
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libnpp/src)

# Library setup
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Defaults
set(PLUGIN_TESTS FALSE CACHE BOOL "Build and run tests")
set(PLUGIN_VALGRIND FALSE CACHE BOOL "Build and run tests using valgrind")

# Platform specific
if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
endif()

# Build library (dynamic)
add_library(plugin SHARED ${SOURCES})
target_link_libraries(plugin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
set_target_properties(plugin PROPERTIES OUTPUT_NAME plugin)
set_target_properties(plugin PROPERTIES BUILD_WITH_INSTALL_RPATH 1 INSTALL_NAME_DIR "@rpath")

# C++11 please
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Install rules
if(NOT INSTALL_DIR)
  set(INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/release)
endif()
install(TARGETS plugin DESTINATION "${INSTALL_DIR}")

# Handle tests
if(PLUGIN_TESTS)
  file(GLOB_RECURSE TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
  enable_testing()
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)
  foreach(TEST ${TESTS})
    get_filename_component(TEST_NAME ${TEST} NAME_WE)
    set(TEST_NAME "${TEST_NAME}")
    add_executable(${TEST_NAME} ${TEST})
    target_link_libraries(${TEST_NAME} plugin)
    add_test(${TEST_NAME} ${TEST_NAME})
    if(PLUGIN_VALGRIND)
      add_test("${TEST_NAME}-valgrind" valgrind --track-origins=yes --leak-check=full --dsymutil=yes ${TEST_NAME})
    endif()
  endforeach(TEST)
endif()
