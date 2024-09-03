# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyDataStructures VERSION 1.0)

# Include directory
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create an executable
add_executable(MyDataStructures ${SOURCES})

