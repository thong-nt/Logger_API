#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <fstream>
#include "Logger.hpp" // Include your logger header file here

// Test the logging functionality
TEST(LoggerTest, LoggingTest) {
    // Define a test log file name
    const std::string testLogFileName = "test_log.txt";

    // Get a reference to the logger instance
    Log::ExportMode::SynchronousLog& logger = Log::ExportMode::SynchronousLog::getInstance(testLogFileName);

    // Add some log messages
    logger.addMessage(Log::MessageLog::Error("User1", "This is an error message."));
    logger.addMessage(Log::MessageLog::Info("User2", "This is an informational message."));

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // As we're using a test log file, let's open it and check its contents
    std::ifstream logFile(testLogFileName);
    ASSERT_TRUE(logFile.is_open()); // Check if the file is open

    // Read each line in the log file and check if the logged messages are present
    std::string line;
    bool errorLogged = false;
    bool infoLogged = false;

    while (std::getline(logFile, line)) {
        if (line.find("[Error]") && line.find("This is an error message.") ) {
            errorLogged = true;
        }
        if (line.find("[Info]") && line.find("This is an informational message.") ) {
            infoLogged = true;
        }
    }

    // Close the log file after reading
    logFile.close();

    // Check if the logged messages were found in the file
    ASSERT_TRUE(errorLogged);
    ASSERT_TRUE(infoLogged);
}

// Test disabling console and file logging
TEST(LoggerTest, DisableLoggingTest) {
    // Set up the logger instance with a test log file
    const std::string testLogFileName = "test_log_2.txt";
    Log::ExportMode::SynchronousLog& logger = Log::ExportMode::SynchronousLog::getInstance(testLogFileName);

    // Disable console and file logging
    logger.DisableFile(true);

    // Add log messages
    logger.addMessage(Log::MessageLog::Error("User1", "This is an error message."));
    logger.addMessage(Log::MessageLog::Info("User2", "This is an informational message."));

    // Read the test log file
    std::ifstream logFile(testLogFileName);
    
    // Check if the log file is open (file logging is disabled, so it should be closed)
    ASSERT_FALSE(logFile.is_open());

    // Read the log messages and ensure they're not present in the file
    // Example: Check if the log file is empty
    ASSERT_TRUE(logFile.peek() == std::ifstream::traits_type::eof());

    // Close the log file after reading
    logFile.close();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
