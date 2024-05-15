#ifndef LOGGINGLEVEL_HPP
#define LOGGINGLEVEL_HPP

#include <string>
#include <unordered_map>

enum class LoggingLevel {
    DEBUG,
    INFO,
    LATENCY,
    WARNING,
    ERROR
};

// ANSI color codes
#define CC_RESET "\033[0m"
#define CC_RED "\033[31m"
#define CC_GREEN "\033[32m"
#define CC_YELLOW "\033[33m"
#define CC_BLUE "\033[34m"
#define CC_MAGENTA "\033[35m"
#define CC_CYAN "\033[36m"
#define CC_WHITE "\033[37m"

const std::unordered_map<LoggingLevel, std::string> LoggingLevelString{
    {LoggingLevel::DEBUG, "DEBUG"},
    {LoggingLevel::INFO, "INFO"},
    {LoggingLevel::LATENCY, "LATENCY"},
    {LoggingLevel::WARNING, "WARNING"},
    {LoggingLevel::ERROR, "ERROR"},
};

const std::unordered_map<LoggingLevel, const char *> LoggingLevelColor{
    {LoggingLevel::DEBUG, CC_MAGENTA},
    {LoggingLevel::INFO, CC_GREEN},
    {LoggingLevel::LATENCY, CC_CYAN},
    {LoggingLevel::WARNING, CC_BLUE},
    {LoggingLevel::ERROR, CC_RED},
};

#endif