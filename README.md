# Logger_API
C++ Logger - Support Synchronous for multiple threads - Just for fun - Make life easier.
The Logger Library provides functionality for synchronized logging to console and file.

## Features

- **Synchronized Logging:** Allows logging messages to both the console and a file simultaneously in a thread-safe manner.
- **Multiple Log Levels:** Supports various log levels such as Error, Information, Debug, and Alarm.

## Usage

### Initialization

1. Include the `logger_api.hpp` header in your project.
2. Initialize the logger instance:

    ```cpp
    Log::ExportMode::SynchronousLog& logger = Log::ExportMode::SynchronousLog::getInstance("log.txt");
    ```

### Logging Messages

```cpp
// Log an error message
logger.addMessage(Log::Message::Error("Author", "Error message"));

// Log an information message
logger.addMessage(Log::Message::Infor("Author", "Information message"));

// Log a debug message
logger.addMessage(Log::Message::Debug("Author", "Debug message"));

// Log an alarm message
logger.addMessage(Log::Message::Alarm("Author", "Alarm message"));
```

### Multithreaded Usage
The logger instance can be accessed from multiple threads without explicitly passing it:

```cpp 
void threadFunction1() {
    Log::ExportMode::SynchronousLog::getInstance().addMessage(Log::Message::Error("Thread 1", "Error message"));
}

void threadFunction2() {
    Log::ExportMode::SynchronousLog::getInstance().addMessage(Log::Message::Infor("Thread 2", "Information message"));
}

```
### Notes
+ Ensure proper initialization of the logger instance before using it.
+ Adjust log levels and message content according to your requirements.

## Contributors
Thong Nguyen @thong-nt

## License
This project is licensed under the MIT License.
