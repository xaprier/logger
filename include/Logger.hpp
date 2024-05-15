#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>

#include "LoggingLevel.hpp"

class Logger {
  public:
    Logger(LoggingLevel level = LoggingLevel::INFO)
        : m_logLevel{level}, m_saveFile{false} {}
    void enableLogToFile();
    void disableLogToFile();
    void setLogToFile(bool save);
    void log(LoggingLevel level, const std::string &message) const;
    void log(LoggingLevel level, int line, const std::string &message) const;
    void log(LoggingLevel level, const char *func, const std::string &message) const;
    void log(LoggingLevel level, int line, const char *func, const std::string &message) const;
    void log(LoggingLevel level, int line, const char *func) const;
    void log(const std::string &message) const;
    static void log_static(LoggingLevel level, const std::string &message);
    static void log_static(LoggingLevel level, int line,
                           const std::string &message);
    static void log_static(LoggingLevel level, const char *func,
                           const std::string &message);
    static void log_static(LoggingLevel level, int line, const char *func,
                           const std::string &message);
    static void log_static(LoggingLevel level, int line, const char *func);
    static void log_static(const std::string &message);

  private:
    void logToFile(const std::string &message) const;
    void logToFile(LoggingLevel level, const std::string &message) const;
    void logToFile(LoggingLevel level, int line, const std::string &message) const;
    void logToFile(LoggingLevel level, const char *func, const std::string &message) const;
    void logToFile(LoggingLevel level, int line, const char *func, const std::string &message) const;
    void logToFile(LoggingLevel level, int line, const char *func) const;
    bool m_saveFile{};
    LoggingLevel m_logLevel;
};

#endif