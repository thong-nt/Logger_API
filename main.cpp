#include <iostream>
#include <thread>
#include "Logger.hpp"

void threadFunction1() {
    // Access the logger instance and add messages
    Log::ExportMode::SynchronousLog::getInstance().addMessage(Log::MessageLog::Error("Thread 1", "Error message"));
}

void threadFunction2() {
    // Access the logger instance and add messages
    Log::ExportMode::SynchronousLog::getInstance().addMessage(Log::MessageLog::Infor("Thread 2", "Information message"));
}

int main() {
    // Creating log messages of different levels
    Log::ExportMode::File LogFile("Test.log");
    Log::ExportMode::Console(Log::MessageLog::Error("ZeK", "??????"));
    LogFile.dump(Log::MessageLog::Error("ZeK", "??????"));

    LogFile.dump(Log::MessageLog::Alarm("ZeK", "??????"));

    LogFile.dump(Log::MessageLog::Infor("ZeK", "??????"));

    LogFile.dump(Log::MessageLog::Debug("ZeK", "??????"));


    // Initialize the logger instance (only once)
    Log::ExportMode::SynchronousLog& logger = Log::ExportMode::SynchronousLog::getInstance("Test.log");

    // Create threads
    std::thread t1(threadFunction1);
    std::thread t2(threadFunction2);

    // Join threads with the main thread
    t1.join();
    t2.join();

    // Add messages from the main thread
    logger.addMessage(Log::MessageLog::Debug("Main Thread", "Debug message from main thread"));
    logger.addMessage(Log::MessageLog::Alarm("Main Thread", "Alarm message from main thread"));
    logger.addMessage(Log::MessageLog::Infor("Main Thread", "Debug message from main thread"));
    logger.addMessage(Log::MessageLog::Error("Main Thread", "Alarm message from main thread"));

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Add more threads or work as needed...
    return 0;
}
