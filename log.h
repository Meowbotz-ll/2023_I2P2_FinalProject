#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

class Log {
public:
    static void Info(const std::string& message);
    static void Error(const std::string& message);
    // Add more methods for different log levels if needed
};

#endif // LOG_H
