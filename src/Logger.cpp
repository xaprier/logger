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

void Logger::getText(std::string &target,
                     const std::string &message,
                     const std::optional<LoggingLevel> &level,
                     const std::optional<int> &line,
                     const std::optional<const char *> &func) {
    if (level) {
        std::string logLevelStr = LoggingLevelString.at(*level);
        auto color = LoggingLevelColor.at(*level);
        target += std::string(CC_RESET) + "[" + color + logLevelStr + CC_RESET + "] ";
    }

    if (line) {
        target += CC_YELLOW + std::string("Line ") + std::to_string(*line) + ", ";
    }

    if (func) {
        target += CC_YELLOW + std::string(*func) + ": ";
    }

    target += CC_YELLOW + message + CC_RESET;
}

void Logger::log(const std::string &message,
                 const std::optional<LoggingLevel> &level,
                 const std::optional<int> &line,
                 const std::optional<const char *> &func) const {
    std::string prefix{};
    {
        if (this->m_enableTime)
            prefix = this->logTime();

        std::cout << prefix;
        std::scoped_lock<std::mutex> lock(mtx);

        if (level)
            Logger::log_static(message, level, line, func);
        else
            Logger::log_static(message, m_logLevel, line, func);
    }
    this->logToFile(message, prefix, level, line, func);
}

void Logger::log_static(const std::string &message,
                        const std::optional<LoggingLevel> &level,
                        const std::optional<int> &line,
                        const std::optional<const char *> &func) {
    std::string output{};
    getText(output, message, level, line, func);
    std::cout << output << std::endl;
}

void Logger::logToFile(const std::string &message,
                       const std::optional<std::string> &time,
                       const std::optional<LoggingLevel> &level,
                       const std::optional<int> &line,
                       const std::optional<const char *> &func) const {
    std::string output{};
    getText(output, message, level, line, func);
    // we should add output of level if there is not exists
    if (!level) {
        std::string logLevelStr = LoggingLevelString.at(m_logLevel);
        auto color = LoggingLevelColor.at(m_logLevel);
        auto text = std::string(std::string(CC_RESET) + "[" + color + logLevelStr + CC_RESET + "] ");
        output.insert(0, text);
    }

    if (this->m_enableTime && time) {
        output.insert(0, *time);
    }

    removeAnsiCodes(output);
    {
        std::scoped_lock<std::mutex> lock(mtx);
        if (this->m_saveFile) {
            std::ofstream outputFile(m_fileName, std::ios_base::app);
            if (outputFile.is_open()) {
                outputFile << output << std::endl;
                outputFile.close();
            } else {
                std::string logLevelStr = LoggingLevelString.at(LoggingLevel::ERROR);
                this->log_static("Unable to open log file!", LoggingLevel::ERROR, __LINE__, __PRETTY_FUNCTION__);
            }
        }
    }
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