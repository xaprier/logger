#ifndef LOGGINGTIMER_HPP
#define LOGGINGTIMER_HPP

#include <chrono>
#include <string>

#include "Logger.hpp"
#include "LoggingLevel.hpp"

class LoggingTimer : public Logger {
  public:
    LoggingTimer(const char *function = "") : Logger(LoggingLevel::LATENCY), m_function(function) {
        this->start();
    }

    ~LoggingTimer() {
        this->finish();
    }

    inline void start() { m_start_time = std::chrono::high_resolution_clock::now(); }

    void finish() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_start_time);
        long estimated_ms = duration.count();
        std::string functionName = std::string(m_function);
        this->log(functionName + ": Estimated " + std::to_string(estimated_ms) + "ms", LoggingLevel::LATENCY);
    }

  private:
    const char *m_function;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
};

#endif  // LOGGINGTIMER_HPP