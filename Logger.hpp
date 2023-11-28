#ifndef LOGGER_API_HPP
#define LOGGER_API_HPP

#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <iostream>
#include <fstream>
#include <condition_variable>

namespace Log 
{
    namespace MessageLog {

        std::string Error(std::string author, std::string content) {
            return "::[" + author + "]::[Error] -- " + content;
        };

        std::string Infor(std::string author, std::string content) {
            return "::[" + author + "]::[Infor] -- " + content;
        };

        std::string Debug(std::string author, std::string content) {
            return "::[" + author + "]::[Debug] -- " + content;
        };

        std::string Alarm(std::string author, std::string content) {
            return "::[" + author + "]::[Alarm] -- " + content;
        };
    }

    namespace ExportMode {

        std::string _GetCurrentTime() 
        {
            std::time_t t = std::time(nullptr);
            constexpr size_t bufferSize = 100; // Define a buffer size
            char timestamp[bufferSize];
            std::strftime(timestamp, bufferSize, "[%Y-%m-%d %H:%M:%S]", std::localtime(&t));
            return std::string(timestamp); // Convert char array to std::string
        };

        void Console(std::string message){
            std::cout << _GetCurrentTime() << message << std::endl;
        };

        class File{
        private:
            std::string filename = "";
        public:
            File(std::string _filename): filename(_filename){};
            ~File(){};

            void dump(std::string message) {
                std::ofstream logFile;
                logFile.open(filename, std::ios::out | std::ios::app);
                if (logFile.is_open())
                {
                    logFile << _GetCurrentTime() << message << std::endl;
                }
                logFile.close();
            };
        };

    class SynchronousLog {
        private:
            std::queue<std::string> messageQueue;
            std::mutex mtx;
            std::condition_variable cv;
            bool stopRequested = false;
            std::string _filename = ""; // Instance of the File class for logging to file

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
                    Console(message);
                    if (logFile.is_open())
                    {
                        logFile << _GetCurrentTime() << message << "\n";
                    }
                }
                logFile.close();
            }

        public:
            SynchronousLog(const std::string& filename) : _filename(filename) {
                std::thread t(&SynchronousLog::loggingThreadFunc, this);
                t.detach();
            }

            ~SynchronousLog() {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    stopRequested = true;
                }
                cv.notify_one();
                messageQueue.empty();
            }

            void addMessage(const std::string& message) {
                std::lock_guard<std::mutex> lock(mtx);
                messageQueue.push(message);
                cv.notify_one();
            }

            static SynchronousLog& getInstance(const std::string& filename = "log.txt") {
                static SynchronousLog instance(filename);
                return instance;
            }
        };
    }
}

#endif