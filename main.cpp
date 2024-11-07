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
    Logger logger(LoggingLevel::DEBUG);
    logger.log(5.15, std::nullopt, __LINE__, __PRETTY_FUNCTION__);
}

void testLogError() {
    Logger logger(LoggingLevel::ERROR, "log.txt", true, true);
    logger.log(123);
}

void testLogInfo() {
    Logger logger(LoggingLevel::INFO);
    logger.log("Log Message");
}

void testLogWarn() {
    Logger logger(LoggingLevel::WARNING, "log.txt", true, true);
    logger.log("Log Message");
    logger.log(std::string("Log Message"));
}

void testLogLatency() {
    Logger logger(LoggingLevel::LATENCY);
    logger.log("Log Message");
}

void testLogTesting() {
    Logger logger(LoggingLevel::TESTING, "log.txt", true, true);
    logger.log("Log Message", LoggingLevel::TESTING);
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));  // NOLINT
    logger.log("Log Message", LoggingLevel::TESTING);
}

void testLogContainers() {
    Logger logger;

    // Define various containers
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::vector<std::string> vec = {"apple", "banana", "cherry"};
    std::list<double> lst = {3.14, 1.618, 2.718};

    // Log each container
    logger.log(arr);  // Uses the specialized operator<< for std::array
    logger.log(vec);  // Uses the general operator<< for std::vector
    logger.log(lst);  // Uses the general operator<< for std::list
}

void testLogColorized() {
    testLogDebug();
    testLogError();
    testLogInfo();
    testLogWarn();
    testLogLatency();
    testLogTesting();
    testLogContainers();
}

void testTimer() {
    LoggingTimer timer(__PRETTY_FUNCTION__);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main() {
    testLogColorized();
    return 0;
}
