cmake_minimum_required(VERSION 3.10)
project(Structura VERSION 1.0)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create the library
add_library(structura ${SOURCES})

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit tests
enable_testing()
add_executable(runTests tests/test_vector.cpp tests/test_stack.cpp tests/test_queue.cpp tests/test_linked_list.cpp)
target_link_libraries(runTests gtest gtest_main structura)

include(GoogleTest)
gtest_discover_tests(runTests)

