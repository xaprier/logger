#ifndef LOGGINGTIMER_HPP
#define LOGGINGTIMER_HPP

#include <chrono>
#include <string>

#include "Logger.hpp"
#include "LoggingLevel.hpp"

/**
 * @class LoggingTimer
 * @brief A specialized logger class to measure and log the execution time of a function.
 *
 * The LoggingTimer class automatically starts a timer upon creation and logs the elapsed time
 * when the object is destroyed. This is useful for measuring the latency of specific code blocks.
 */
class LoggingTimer {
  public:
    /**
     * @brief Constructs a LoggingTimer object and starts the timer.
     * @param function The name of the function being timed (default is an empty string).
     */
    LoggingTimer(const char *function = "") : m_function(function) {
        m_logger.SetLogLevel(LoggingLevel::LATENCY);
        this->start();
    }

    /**
     * @brief Destructor, automatically stops the timer and logs the elapsed time.
     */
    ~LoggingTimer() {
        this->finish();
    }

    /**
     * @brief Starts the timer.
     */
    inline void start() {
        m_logger.Log("Started: " + std::string(m_function), LoggingLevel::LATENCY);
        m_start_time = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief Stops the timer and logs the elapsed time in milliseconds.
     */
    void finish() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_start_time);
        long estimated_ms = duration.count();
        m_logger.Log("End: " + std::string(m_function) + ": Estimated " + std::to_string(estimated_ms) + "ms", LoggingLevel::LATENCY);
    }

  private:
    Logger &m_logger = Logger::GetInstance();                                  ///< Reference to the singleton Logger instance.
    const char *m_function;                                                    ///< The name of the function being timed.
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;  ///< The starting time point of the timer.
};

#endif  // LOGGINGTIMER_HPP