#include "log.h"

void Log::Info(const std::string& message) {
    std::cout << "INFO: " << message << std::endl;
}

void Log::Error(const std::string& message) {
    std::cerr << "ERROR: " << message << std::endl;
}
