#include <chrono>
#include <iostream>
#include <thread>

#include "Logger.hpp"
#include "LoggingLevel.hpp"
#include "LoggingTimer.hpp"

void testLogWarn() {
    Logger logger(LoggingLevel::WARNING);
    logger.log("Log Message");
}
void testLogError() {
    Logger logger(LoggingLevel::ERROR);
    logger.log("Log Message");
}
void testLogDebug() {
    Logger logger(LoggingLevel::DEBUG);
    logger.log("Log Message");
}
void testLogLatency() {
    Logger logger(LoggingLevel::LATENCY);
    logger.log("Log Message");
}
void testLogInfo() {
    Logger logger(LoggingLevel::INFO);
    logger.log("Log Message");
}
void testLogTesting() {
    Logger logger(LoggingLevel::TESTING, "log.txt", true, true);
    logger.log(LoggingLevel::TESTING, "Log Message");
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));  // NOLINT
    logger.log(LoggingLevel::TESTING, "Log Message");
}

void testLogColorized() {
    testLogDebug();
    testLogError();
    testLogInfo();
    testLogWarn();
    testLogLatency();
    testLogTesting();
}

void testTimer() {
    LoggingTimer timer(__PRETTY_FUNCTION__);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main() {
    testLogColorized();
    return 0;
}
