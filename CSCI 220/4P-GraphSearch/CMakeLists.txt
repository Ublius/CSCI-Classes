# CMakeLists.txt for the sorting project
cmake_minimum_required(VERSION 3.14)

# Ensure we can use gdb
set(CMAKE_BUILD_TYPE Debug)

project(project5-bfs)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

# Retrieve the googletest test framework
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Copy large graph file to build directory
file(COPY large_graph.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_compile_options(-pedantic -Wall -Wextra)

add_executable(run-main graph.h graph.cpp main.cpp)
add_executable(run-tests graph.h graph.cpp tests/tests.cpp)

target_link_libraries(run-tests GTest::gtest_main)
