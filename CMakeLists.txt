cmake_minimum_required(VERSION 3.10)
project(multithreaded_generator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(external/googletest/googletest/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Google Test Integration
# Use the googletest submodule from the 'external' directory
add_subdirectory(external/googletest)

enable_testing()

add_executable(${PROJECT_NAME}_tests tests/test_thread_safe_queue.cpp)

target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)