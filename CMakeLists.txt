cmake_minimum_required(VERSION 3.0)

project(Logger_API)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Locate necessary packages
find_package(Threads REQUIRED)

# Add executable
add_executable(Logger_API main.cpp)

# Link necessary libraries
target_link_libraries(Logger_API Threads::Threads)
