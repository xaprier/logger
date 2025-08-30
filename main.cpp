#include <chrono>
#include <list>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "Logger.hpp"
#include "LoggingLevel.hpp"
#include "LoggingTimer.hpp"

void testLogDebug() {
    auto &logger = Logger::GetInstance(LoggingLevel::DEBUG);
    logger.Log(5.15, std::nullopt, __LINE__, __PRETTY_FUNCTION__);
}

void testLogError() {
    auto &logger = Logger::GetInstance(LoggingLevel::ERROR, "log.txt", true, true);
    logger.Log(123);
}

void testLogInfo() {
    auto &logger = Logger::GetInstance(LoggingLevel::INFO);
    logger.Log("Log Message");
}

void testLogWarn() {
    auto &logger = Logger::GetInstance(LoggingLevel::WARNING, "log.txt", true, true);
    logger.Log("Log Message");
    logger.Log(std::string("Log Message"));
}

void testLogLatency() {
    auto &logger = Logger::GetInstance(LoggingLevel::LATENCY);
    logger.Log("Log Message");
}

void testLogTesting() {
    auto &logger = Logger::GetInstance(LoggingLevel::TESTING, "log.txt", true, true);
    logger.Log("Log Message", LoggingLevel::TESTING);
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));  // NOLINT
    logger.Log("Log Message", LoggingLevel::TESTING);
}

void testLogContainers() {
    auto &logger = Logger::GetInstance();

    // Define various containers
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::vector<std::string> vec = {"apple", "banana", "cherry"};
    std::list<double> lst = {3.14, 1.618, 2.718};

    // Log each container
    logger.Log(arr);  // Uses the specialized operator<< for std::array
    logger.Log(vec);  // Uses the general operator<< for std::vector
    logger.Log(lst);  // Uses the general operator<< for std::list
}

void testLoggingTimer() {
    LoggingTimer timer(__PRETTY_FUNCTION__);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void testLog() {
    testLogDebug();
    testLogError();
    testLogInfo();
    testLogWarn();
    testLogLatency();
    testLogTesting();
    testLogContainers();
    testLoggingTimer();
}

int main() {
    Logger::GetInstance(LoggingLevel::DEBUG, "log.txt", true, true, true);
    testLog();
    return 0;
}
