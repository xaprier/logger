#include "Logger.hpp"

#include <chrono>
#include <mutex>
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

void Logger::log(LoggingLevel level, const std::string &message) const {
    {
        std::scoped_lock<std::mutex> lock(mtx);
        std::string logLevelStr = LoggingLevelString.at(level);
        auto color = LoggingLevelColor.at(level);
        if (this->m_enableTime) this->logTime();
        std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
                  << CC_YELLOW << ": " << message << CC_RESET << std::endl;
    }

    this->logToFile(level, message);
}

void Logger::log(const std::string &message) const {
    {
        std::scoped_lock<std::mutex> lock(mtx);
        std::string logLevelStr = LoggingLevelString.at(m_logLevel);
        auto color = LoggingLevelColor.at(m_logLevel);
        if (this->m_enableTime) this->logTime();
        std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
                  << CC_YELLOW << ": " << message << CC_RESET << std::endl;
    }

    this->logToFile(m_logLevel, message);
}

void Logger::log(LoggingLevel level, int line,
                 const std::string &message) const {
    {
        std::scoped_lock<std::mutex> lock(mtx);
        std::string logLevelStr = LoggingLevelString.at(level);
        auto color = LoggingLevelColor.at(level);
        if (this->m_enableTime) this->logTime();
        std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
                  << CC_YELLOW << "Line " << line << ": " << message << CC_RESET
                  << std::endl;
    }

    this->logToFile(level, line, message);
}

void Logger::log(LoggingLevel level, const char *func,
                 const std::string &message) const {
    {
        std::scoped_lock<std::mutex> lock(mtx);
        std::string logLevelStr = LoggingLevelString.at(level);
        auto color = LoggingLevelColor.at(level);
        if (this->m_enableTime) this->logTime();
        std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
                  << CC_YELLOW << func << ": " << message << CC_RESET << std::endl;
    }

    this->logToFile(level, func, message);
}

void Logger::log(LoggingLevel level, int line, const char *func,
                 const std::string &message) const {
    {
        std::scoped_lock<std::mutex> lock(mtx);
        std::string logLevelStr = LoggingLevelString.at(level);
        auto color = LoggingLevelColor.at(level);
        if (this->m_enableTime) this->logTime();
        std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
                  << CC_YELLOW << "Line " << line << ", " << func << ": " << message
                  << CC_RESET << std::endl;
    }

    this->logToFile(level, line, func, message);
}

void Logger::log(LoggingLevel level, int line, const char *func) const {
    {
        std::scoped_lock<std::mutex> lock(mtx);
        std::string logLevelStr = LoggingLevelString.at(level);
        auto color = LoggingLevelColor.at(level);
        if (this->m_enableTime) this->logTime();
        std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
                  << CC_YELLOW << "Line " << line << ", " << func << CC_RESET
                  << std::endl;
    }

    this->logToFile(level, line, func);
}

void Logger::log_static(LoggingLevel level, const std::string &message) {
    std::string logLevelStr = LoggingLevelString.at(level);
    auto color = LoggingLevelColor.at(level);
    std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
              << CC_YELLOW << ": " << message << std::endl;
}

void Logger::log_static(LoggingLevel level, int line,
                        const std::string &message) {
    std::string logLevelStr = LoggingLevelString.at(level);
    auto color = LoggingLevelColor.at(level);
    std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
              << CC_YELLOW << "Line " << line << ": " << message << CC_RESET
              << std::endl;
}

void Logger::log_static(LoggingLevel level, const char *func,
                        const std::string &message) {
    std::string logLevelStr = LoggingLevelString.at(level);
    auto color = LoggingLevelColor.at(level);
    std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
              << CC_YELLOW << func << ": " << message << CC_RESET << std::endl;
}

void Logger::log_static(LoggingLevel level, int line, const char *func,
                        const std::string &message) {
    std::string logLevelStr = LoggingLevelString.at(level);
    auto color = LoggingLevelColor.at(level);
    std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
              << CC_YELLOW << "Line " << line << ", " << func << ": " << message
              << CC_RESET << std::endl;
}

void Logger::log_static(LoggingLevel level, int line, const char *func) {
    std::string logLevelStr = LoggingLevelString.at(level);
    auto color = LoggingLevelColor.at(level);
    std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
              << CC_YELLOW << "Line " << line << ", " << func << CC_RESET
              << std::endl;
}

void Logger::log_static(const std::string &message) {
    std::string logLevelStr =
        LoggingLevelString.at(LoggingLevel::INFO);  // default info
    auto color = LoggingLevelColor.at(LoggingLevel::INFO);
    std::cout << CC_RESET << "[" << color << logLevelStr << CC_RESET << "] "
              << CC_YELLOW << ": " << message << CC_RESET << std::endl;
}

void Logger::logToFile(const std::string &message) const {
    std::scoped_lock<std::mutex> lock(mtx);
    if (this->m_saveFile) {
        std::ofstream outputFile(m_fileName, std::ios_base::app);
        if (outputFile.is_open()) {
            outputFile << message;
            outputFile.close();
        } else {
            std::string logLevelStr = LoggingLevelString.at(LoggingLevel::ERROR);
            std::cout << "[" << logLevelStr << "] "
                      << "Line " << __LINE__ << ", " << __PRETTY_FUNCTION__ << ": "
                      << "Unable to open log file!" << std::endl;
        }
    }
}

void Logger::logToFile(LoggingLevel level, const std::string &message) const {
    const std::string logLevelStr = LoggingLevelString.at(level);
    const std::string log = "[" + logLevelStr + "]" + ": " + message + "\n";
    this->logToFile(log);
}

void Logger::logToFile(LoggingLevel level, int line, const std::string &message) const {
    const std::string logLevelStr = LoggingLevelString.at(level);
    const std::string log = "[" + logLevelStr + "] " + "Line " + std::to_string(line) + ": " + message + "\n";
    this->logToFile(log);
}

void Logger::logToFile(LoggingLevel level, const char *func, const std::string &message) const {
    const std::string logLevelStr = LoggingLevelString.at(level);
    const std::string log = "[" + logLevelStr + "] " + func + ": " + message + "\n";
    this->logToFile(log);
}

void Logger::logToFile(LoggingLevel level, int line, const char *func, const std::string &message) const {
    const std::string logLevelStr = LoggingLevelString.at(level);
    const std::string log = "[" + logLevelStr + "] " + "Line " + std::to_string(line) + ", " + func + ": " + message + "\n";
    this->logToFile(log);
}

void Logger::logToFile(LoggingLevel level, int line, const char *func) const {
    const std::string logLevelStr = LoggingLevelString.at(level);
    const std::string log = "[" + logLevelStr + "] " + "Line " + std::to_string(line) + ", " + func + "\n";
    this->logToFile(log);
}

void Logger::logTime() const {
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

    std::cout << CC_RESET << "[" << CC_MAGENTA << execution << CC_RESET << unit << "]";
}