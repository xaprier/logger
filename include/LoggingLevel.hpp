#ifndef LOGGINGLEVEL_HPP
#define LOGGINGLEVEL_HPP

#include <string>
#include <unordered_map>

enum class LoggingLevel { DEBUG,
                          INFO,
                          TESTING,
                          LATENCY,
                          WARNING,
                          ERROR };

const std::unordered_map<LoggingLevel, std::string> LoggingLevelString{
    {LoggingLevel::DEBUG, "DEBUG"},
    {LoggingLevel::INFO, "INFO"},
    {LoggingLevel::TESTING, "TESTING"},
    {LoggingLevel::LATENCY, "LATENCY"},
    {LoggingLevel::WARNING, "WARNING"},
    {LoggingLevel::ERROR, "ERROR"},
};

;

#endif