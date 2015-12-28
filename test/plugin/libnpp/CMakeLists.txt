## Project
cmake_minimum_required(VERSION 3.1)
project(npp)

# Defaults
set(NPP_TESTS FALSE CACHE BOOL "Build and run tests")

# Platform specific
if(APPLE)
  if("${CMAKE_OSX_ARCHITECTURES}" EQUAL "")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  endif()
endif()

# C++11 please
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Handle tests
if(NPP_TESTS)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
  file(GLOB_RECURSE TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
  enable_testing()
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)
  foreach(TEST ${TESTS})
    get_filename_component(TEST_NAME ${TEST} NAME_WE)
    set(TEST_NAME "${TEST_NAME}")
    add_executable(${TEST_NAME} ${TEST})
    target_link_libraries(${TEST_NAME})
    add_test(${TEST_NAME} ${TEST_NAME})
  endforeach(TEST)
endif()
