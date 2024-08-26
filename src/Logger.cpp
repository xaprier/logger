#include "Logger.hpp"

#include <chrono>
#include <mutex>
#include <optional>
#include <string>

#include "LoggingLevel.hpp"

void Logger::enableLogToFile() {
    this->m_saveFile = true;
}

void Logger::disableLogToFile() {
    this->m_saveFile = false;
}

void Logger::setLogToFile(bool save) {
    this->m_saveFile = save;
}

void Logger::enableTimer() {
    this->m_enableTime = true;
}

void Logger::disableTimer() {
    this->m_enableTime = false;
}

void Logger::setTimer(bool timingOn) {
    this->m_enableTime = timingOn;
}

std::string Logger::logTime() const {
    auto duration = std::chrono::steady_clock::now() - start_time;
    long double execution;
    std::string unit;
    if (duration < std::chrono::microseconds(1)) {
        execution = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        unit = "ns";
    } else if (duration < std::chrono::milliseconds(1)) {
        execution = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        unit = "μs";
    } else if (duration < std::chrono::seconds(1)) {
        execution = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        unit = "ms";
    } else {
        execution = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        execution /= 1000.0;
        unit = "s";
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << execution;  // Set precision to 3 decimal places

    std::string output = std::string(CC_RESET) + "[" + std::string(CC_MAGENTA) + oss.str() + std::string(CC_RESET) + unit + "]";
    return output;
}

void Logger::removeAnsiCodes(std::string &text) {
    std::regex ansiRegex(R"(\x1B\[[0-9;]*[a-zA-Z])");
    text = std::regex_replace(text, ansiRegex, "");
}