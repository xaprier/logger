#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>
#include <regex>
#include <string>

#include "LoggingLevel.hpp"

//! IMPORTANT You will have to include the file in main function's file to get
//! real execution time.
static const auto start_time = std::chrono::steady_clock::now();

class Logger {
  public:
    Logger(LoggingLevel level = LoggingLevel::INFO,
           std::string fileName = "log.txt", bool saveToFile = false,
           bool enableTimer = false)
        : m_logLevel{level}, m_fileName{fileName}, m_saveFile{saveToFile}, m_enableTime{enableTimer} {}

    ~Logger() = default;

    Logger(const Logger &other)
        : m_fileName(other.m_fileName), m_enableTime(other.m_enableTime), m_saveFile(other.m_saveFile), m_logLevel(other.m_logLevel) {}

    Logger &operator=(const Logger &other) {
        if (this != &other) {
            m_fileName = std::move(other.m_fileName);
            m_enableTime = other.m_enableTime;
            m_saveFile = other.m_saveFile;
            m_logLevel = other.m_logLevel;
        }
        return *this;
    }

    Logger(Logger &&other) noexcept
        : m_fileName(std::move(other.m_fileName)),
          m_enableTime(other.m_enableTime),
          m_saveFile(other.m_saveFile),
          m_logLevel(other.m_logLevel) {
        // Since mutexes cannot be moved, we don't move mtx.
    }

    Logger &operator=(Logger &&other) noexcept {
        if (this != &other) {
            m_fileName = std::move(other.m_fileName);
            m_enableTime = other.m_enableTime;
            m_saveFile = other.m_saveFile;
            m_logLevel = other.m_logLevel;
        }
        return *this;
    }

    void enableLogToFile();
    void disableLogToFile();
    void setLogToFile(bool save);
    void enableTimer();
    void disableTimer();
    void setTimer(bool timingOn);

    void log(const std::string &message,
             const std::optional<LoggingLevel> &level = std::nullopt,
             const std::optional<int> &line = std::nullopt,
             const std::optional<const char *> &func = std::nullopt) const;

    static void log_static(const std::string &message,
                           const std::optional<LoggingLevel> &level = std::nullopt,
                           const std::optional<int> &line = std::nullopt,
                           const std::optional<const char *> &func = std::nullopt);

  private:
    void logTime() const;
    static void removeAnsiCodes(std::string &text);

    static void getText(std::string &target,
                        const std::string &message,
                        const std::optional<LoggingLevel> &level = std::nullopt,
                        const std::optional<int> &line = std::nullopt,
                        const std::optional<const char *> &func = std::nullopt);
    void logToFile(const std::string &message,
                   const std::optional<LoggingLevel> &level = std::nullopt,
                   const std::optional<int> &line = std::nullopt,
                   const std::optional<const char *> &func = std::nullopt) const;
    mutable std::mutex mtx;  // Mutex for thread-safe logging
    std::string m_fileName;
    bool m_enableTime;
    bool m_saveFile;
    LoggingLevel m_logLevel;
};

#endif