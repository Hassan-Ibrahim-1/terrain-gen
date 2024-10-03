#include "error_handler.hpp"

bool ErrorHandler::had_error = false;

void ErrorHandler::error(const std::string& message, const std::string& location) {
    report(message, location);
    had_error = true;
}

void ErrorHandler::report(const std::string& message, const std::string& location) {
    fprintf(stderr, "\033[1;31m[ERROR]\033[0m at [%s]: %s\n", location.c_str(), message.c_str());
}

