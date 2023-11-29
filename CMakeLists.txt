cmake_minimum_required(VERSION 3.0)

project(Logger_API)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Locate necessary packages
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# Add executable for your main code
add_executable(Logger_API main.cpp)

# Link necessary libraries
target_link_libraries(Logger_API Threads::Threads)

# Add an executable for testing
add_executable(LoggerAPITests UnitTest.cpp)  # Change test.cpp to your actual test file name
target_link_libraries(LoggerAPITests GTest::GTest GTest::Main)

# Enable testing
enable_testing()

# Add the test
add_test(NAME LoggerAPITests COMMAND LoggerAPITests)
