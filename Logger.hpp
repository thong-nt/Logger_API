#ifndef LOGGER_API_HPP
#define LOGGER_API_HPP

#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <iostream>
#include <fstream>
#include <condition_variable>

namespace Log {

    // Namespace for different log message types
    namespace MessageLog {

        // Generate Error log message
        std::string Error(std::string author, std::string content) {
            return "::[" + author + "]::[Error] -- " + content;
        };

        // Generate Information log message
        std::string Info(std::string author, std::string content) {
            return "::[" + author + "]::[Info] -- " + content;
        };

        // Generate Debug log message
        std::string Debug(std::string author, std::string content) {
            return "::[" + author + "]::[Debug] -- " + content;
        };

        // Generate Alarm log message
        std::string Alarm(std::string author, std::string content) {
            return "::[" + author + "]::[Alarm] -- " + content;
        };
    }

    // Namespace for different log export modes
    namespace ExportMode {

        // Get current system time as a formatted string
        std::string _GetCurrentTime() {
            std::time_t t = std::time(nullptr);
            constexpr size_t bufferSize = 100; // Define a buffer size
            char timestamp[bufferSize];
            std::strftime(timestamp, bufferSize, "[%Y-%m-%d %H:%M:%S]", std::localtime(&t));
            return std::string(timestamp); // Convert char array to std::string
        };

        // Output log messages to the console
        void Console(std::string message) {
            std::cout << _GetCurrentTime() << message << std::endl;
        };

        // Class for logging to a file
        class File {
        private:
            std::string filename = "";

        public:
            File(std::string _filename) : filename(_filename) {};

            // Append a log message to the file
            void dump(std::string message) {
                std::ofstream logFile;
                logFile.open(filename, std::ios::out | std::ios::app);
                if (logFile.is_open()) {
                    logFile << _GetCurrentTime() << message << std::endl;
                }
                logFile.close();
            };
        };

        // Class for synchronous logging
        class SynchronousLog {
        private:
            std::queue<std::string> messageQueue;
            std::mutex mtx;
            std::condition_variable cv;
            bool stopRequested = false;
            bool _disableConsole = false;
            bool _disableFile = false;
            std::string _filename = "";

            // Logging thread function
            void loggingThreadFunc() {
                std::ofstream logFile;
                logFile.open(_filename, std::ios::out | std::ios::app);

                while (true) {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] { return !messageQueue.empty() || stopRequested; });

                    if (stopRequested && messageQueue.empty()) {
                        break;
                    }

                    std::string message = messageQueue.front();
                    messageQueue.pop();
                    lock.unlock();

                    // Select output method: console or file
                    if (!_disableConsole) {
                        Console(message);
                    }
                    if (!_disableFile && logFile.is_open()) {
                        logFile << _GetCurrentTime() << message << "\n";
                    }
                }

                logFile.close();
            }

        public:
            // Constructor
            SynchronousLog(const std::string& filename) : _filename(filename) {
                std::thread t(&SynchronousLog::loggingThreadFunc, this);
                t.detach();
            }

            // Destructor
            ~SynchronousLog() {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    stopRequested = true;
                }
                cv.notify_one();
                messageQueue.empty();
            }

            // Add a log message to the queue
            void addMessage(const std::string& message) {
                std::lock_guard<std::mutex> lock(mtx);
                messageQueue.push(message);
                cv.notify_one();
            }

            // Disable console logging
            void DisableConsole(bool var) {
                _disableConsole = var;
            }

            // Disable file logging
            void DisableFile(bool var) {
                _disableFile = var;
            }

            // Get an instance of SynchronousLog
            static SynchronousLog& getInstance(const std::string& filename = "log.txt") {
                static SynchronousLog instance(filename);
                return instance;
            }
        };
    }
}

#endif
