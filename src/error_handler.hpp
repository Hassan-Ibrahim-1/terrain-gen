#pragma once

#include <string>

namespace ErrorHandler {

extern bool had_error;

void error(const std::string& message, const std::string& location = "LOCATION NOT SPECIFIED");
void report(const std::string& message, const std::string& location);

}

