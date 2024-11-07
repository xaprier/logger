#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <regex>
#include <string>

#include "LoggingHelpers.hpp"
#include "LoggingLevel.hpp"

//! IMPORTANT You will have to include the file in main function's file to get
//! real execution time.
static const auto start_time = std::chrono::steady_clock::now();

/**
 * @class Logger
 * @brief A logging class that supports logging messages to the console and optionally to a file.
 *
 * The Logger class provides functionality for logging messages with different levels,
 * including options for time-stamping and saving logs to a file.
 */
class Logger {
  public:
    /**
     * @brief Constructor for the Logger class.
     * @param level The logging level (default: LoggingLevel::INFO).
     * @param fileName The name of the log file (default: "log.txt").
     * @param saveToFile Flag to enable/disable saving logs to a file (default: false).
     * @param enableTimer Flag to enable/disable time-stamping of logs (default: false).
     */
    Logger(LoggingLevel level = LoggingLevel::INFO,
           std::string fileName = "log.txt", bool saveToFile = false,
           bool enableTimer = false)
        : m_logLevel{level}, m_fileName{fileName}, m_saveFile{saveToFile}, m_enableTime{enableTimer} {}

    /**
     * @brief Default destructor for the Logger class.
     */
    ~Logger() = default;

    /**
     * @brief Copy constructor for the Logger class.
     * @param other The Logger object to copy from.
     */
    Logger(const Logger &other)
        : m_fileName(other.m_fileName), m_enableTime(other.m_enableTime), m_saveFile(other.m_saveFile), m_logLevel(other.m_logLevel) {}

    /**
     * @brief Copy assignment operator for the Logger class.
     * @param other The Logger object to copy from.
     * @return A reference to the current Logger object.
     */
    Logger &operator=(const Logger &other) {
        if (this != &other) {
            m_fileName = std::move(other.m_fileName);
            m_enableTime = other.m_enableTime;
            m_saveFile = other.m_saveFile;
            m_logLevel = other.m_logLevel;
        }
        return *this;
    }

    /**
     * @brief Move constructor for the Logger class.
     * @param other The Logger object to move from.
     */
    Logger(Logger &&other) noexcept
        : m_fileName(std::move(other.m_fileName)),
          m_enableTime(other.m_enableTime),
          m_saveFile(other.m_saveFile),
          m_logLevel(other.m_logLevel) {
        // Since mutexes cannot be moved, we don't move mtx.
    }

    /**
     * @brief Move assignment operator for the Logger class.
     * @param other The Logger object to move from.
     * @return A reference to the current Logger object.
     */
    Logger &operator=(Logger &&other) noexcept {
        if (this != &other) {
            m_fileName = std::move(other.m_fileName);
            m_enableTime = other.m_enableTime;
            m_saveFile = other.m_saveFile;
            m_logLevel = other.m_logLevel;
        }
        return *this;
    }

    /**
     * @brief Enables logging to a file.
     */
    void enableLogToFile();

    /**
     * @brief Disables logging to a file.
     */
    void disableLogToFile();

    /**
     * @brief Sets the logging to a file option.
     * @param save Flag to enable/disable saving logs to a file.
     */
    void setLogToFile(bool save);

    /**
     * @brief Enables the timer for log messages.
     */
    void enableTimer();

    /**
     * @brief Disables the timer for log messages.
     */
    void disableTimer();

    /**
     * @brief Sets the timer option for log messages.
     * @param timingOn Flag to enable/disable time-stamping of logs.
     */
    void setTimer(bool timingOn);

    /**
     * @brief Logs a message to the console and optionally to a file.
     * @param message The message to log.
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    void log(const T &message,
             const std::optional<LoggingLevel> &level = std::nullopt,
             const std::optional<int> &line = std::nullopt,
             const std::optional<const char *> &func = std::nullopt) const {
        // Convert the message to a string using std::ostringstream
        std::ostringstream oss;
        if constexpr (std::is_same<T, std::string>::value) {
            oss << message.c_str();  // std::string represents like ['a', 'b', 'c'], we will get char* to not happen
        } else {
            oss << message;
        }
        std::string messageStr = oss.str();

        std::string prefix{};
        {
            if (this->m_enableTime)
                prefix = this->logTime();

            std::cout << prefix;
            std::scoped_lock<std::mutex> lock(mtx);

            if (level)
                Logger::log_static(messageStr, level, line, func);
            else
                Logger::log_static(messageStr, m_logLevel, line, func);
        }
        this->logToFile(messageStr, prefix, level, line, func);
    }

    /**
     * @brief Static function to log a message without creating a Logger instance.
     * @param message The message to log.
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    static void log_static(const T &message,
                           const std::optional<LoggingLevel> &level = std::nullopt,
                           const std::optional<int> &line = std::nullopt,
                           const std::optional<const char *> &func = std::nullopt) {
        std::string output{};
        getText(output, message, level, line, func);
        std::cout << output << std::endl;
    }

  private:
    /**
     * @brief Gets the estimated time as a formatted string.
     * @return A string representing the estimated time.
     */
    std::string logTime() const;

    /**
     * @brief Removes ANSI color codes from a string.
     * @param text The string to remove ANSI codes from.
     */
    static void removeAnsiCodes(std::string &text);

    /**
     * @brief Formats a log message with optional parameters.
     * @param target The target string to store the formatted message.
     * @param message The message to log.
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    static void getText(std::string &target,
                        const T &message,
                        const std::optional<LoggingLevel> &level = std::nullopt,
                        const std::optional<int> &line = std::nullopt,
                        const std::optional<const char *> &func = std::nullopt) {
        if (level) {
            std::string logLevelStr = LoggingLevelString.at(*level);
            auto color = LoggingLevelColor.at(*level);
            target += std::string(CC_RESET) + "[" + color + logLevelStr + CC_RESET + "]";
        }

        if (line) {
            target += std::string(CC_RESET) + "(" + CC_CYAN + "Line " + std::to_string(line.value()) + CC_RESET + ")";
        }

        if (func) {
            target += std::string(CC_RESET) + "{" + CC_BLUE + func.value() + CC_RESET + "}";
        }

        target += ": " + std::string(CC_YELLOW) + message + std::string(CC_RESET);
    }

    /**
     * @brief Logs the message to the given file
     * @param message The message to log.
     * @param time The time of the log (optional).
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    void logToFile(const T &message,
                   const std::optional<std::string> &time = std::nullopt,
                   const std::optional<LoggingLevel> &level = std::nullopt,
                   const std::optional<int> &line = std::nullopt,
                   const std::optional<const char *> &func = std::nullopt) const {
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

    mutable std::mutex mtx;   ///< Mutex for thread-safe logging
    std::string m_fileName;   ///< Name of the log file
    bool m_enableTime;        ///< Flag to enable/disable time-stamping of logs
    bool m_saveFile;          ///< Flag to enable/disable saving logs to a file
    LoggingLevel m_logLevel;  ///< Current logging level
};

#endif