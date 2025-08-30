#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <atomic>
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
static const auto logger_start_time = std::chrono::steady_clock::now();

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
     * @brief Gets the singleton instance of the Logger class.
     * @return A reference to the singleton Logger instance.
     */
    static Logger &GetInstance(LoggingLevel level = LoggingLevel::INFO, std::string fileName = "log.txt", bool saveToFile = false, bool enableTimer = false, bool enable = true) {
        static Logger instance(level, fileName, saveToFile, enableTimer, enable);
        return instance;
    }

    /**
     * @brief Delete the move and copy constructors and assignment operators.
     */
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(Logger &&) = delete;

    /**
     * @brief Getters to check variables
     */
    bool IsLogEnabled() const;
    bool IsTimerEnabled() const;
    bool IsFileSaveEnabled() const;
    bool IsColorEnabled() const;
    LoggingLevel GetLevel() const;
    std::string GetFileName() const;

    /**
     * @brief Enables logging.
     */
    void EnableLog();

    /**
     * @brief Disables logging.
     */
    void DisableLog();

    /**
     * @brief Sets the logging to an enabled/disabled state.
     * @param save Flag to enable/disable logging.
     */
    void SetLog(bool save);

    /**
     * @brief Enables logging to a file.
     */
    void EnableFile();

    /**
     * @brief Disables logging to a file.
     */
    void DisableFile();

    /**
     * @brief Enables the timer for log messages.
     */
    void EnableTimer();

    /**
     * @brief Disables the timer for log messages.
     */
    void DisableTimer();

    /**
     * @brief Set the logging level
     * @param level The logging level to set.
     */
    void SetLogLevel(LoggingLevel level);

    /**
     * @brief Set log file name
     * @param fileName The name of the log file.
     */
    void SetLogFileName(const std::string &fileName);

    /**
     * @brief Sets the logging to a file option.
     * @param save Flag to enable/disable saving logs to a file.
     */
    void SetFile(bool save);

    /**
     * @brief Sets the timer option for log messages.
     * @param timingOn Flag to enable/disable time-stamping of logs.
     */
    void SetTimer(bool timingOn);

    /**
     * @brief Logs a message to the console and optionally to a file.
     * @param message The message to log.
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    void Log(const T &message,
             const std::optional<LoggingLevel> &level = std::nullopt,
             const std::optional<int> &line = std::nullopt,
             const std::optional<const char *> &func = std::nullopt) const {
        // Logging is disabled
        if (!this->m_enable.load()) return;

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
                prefix = this->_LogTime();

            std::scoped_lock<std::mutex> lock(m_mtx);  // lock before printing to console
            std::cout << prefix;

            if (level)
                Logger::Log_Static(messageStr, level, line, func);
            else
                Logger::Log_Static(messageStr, m_logLevel, line, func);
        }

        if (this->m_saveFile) this->_ToFile(messageStr, prefix, level, line, func);
    }

    /**
     * @brief Static function to log a message without creating a Logger instance.
     * @param message The message to log.
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    static void Log_Static(const T &message,
                           const std::optional<LoggingLevel> &level = std::nullopt,
                           const std::optional<int> &line = std::nullopt,
                           const std::optional<const char *> &func = std::nullopt) {
        std::string output{};
        _GetText(output, message, level, line, func);
        std::cout << output << std::endl;
    }

  private:
    /**
     * @brief Constructor for the Logger class.
     * @param level The logging level (default: LoggingLevel::INFO).
     * @param fileName The name of the log file (default: "log.txt").
     * @param saveToFile Flag to enable/disable saving logs to a file (default: false).
     * @param enableTimer Flag to enable/disable time-stamping of logs (default: false).
     */
    Logger(LoggingLevel level = LoggingLevel::INFO, std::string fileName = "log.txt", bool saveToFile = false, bool enableTimer = false, bool enable = true);

    ~Logger();

    /**
     * @brief Gets the estimated time as a formatted string.
     * @return A string representing the estimated time.
     */
    std::string _LogTime() const;

    /**
     * @brief Formats a log message with optional parameters.
     * @param target The target string to store the formatted message.
     * @param message The message to log.
     * @param level The logging level (optional).
     * @param line The line number (optional).
     * @param func The function name (optional).
     */
    template <typename T>
    static void _GetText(std::string &target,
                         const T &message,
                         const std::optional<LoggingLevel> &level = std::nullopt,
                         const std::optional<int> &line = std::nullopt,
                         const std::optional<const char *> &func = std::nullopt) {
        std::ostringstream oss;

        if (level) {
            const auto &logLevelStr = LoggingLevelString.at(*level);
            oss << "[" << logLevelStr << "] ";
        }

        if (line) {
            oss << "(Line " << *line << ") ";
        }

        if (func) {
            oss << "{" << *func << "}";
        }

        oss << ": " << message;

        target += oss.str();
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
    void _ToFile(const T &message,
                 const std::optional<std::string> &time = std::nullopt,
                 const std::optional<LoggingLevel> &level = std::nullopt,
                 const std::optional<int> &line = std::nullopt,
                 const std::optional<const char *> &func = std::nullopt) const {
        std::string output{};
        _GetText(output, message, level, line, func);
        // we should add output of level if there is not exists
        if (!level) {
            std::string logLevelStr = LoggingLevelString.at(m_logLevel);
            auto text = std::string("[" + logLevelStr + "] ");
            output.insert(0, text);
        }

        if (this->m_enableTime && time) {
            output.insert(0, *time);
        }

        {
            std::scoped_lock<std::mutex> lock(m_mtx);
            if (this->m_saveFile) {
                std::ofstream outputFile(m_fileName, std::ios_base::app);
                if (outputFile.is_open()) {
                    outputFile << output << std::endl;
                    outputFile.close();
                } else {
                    std::string logLevelStr = LoggingLevelString.at(LoggingLevel::ERROR);
                    this->Log_Static("Unable to open log file!", LoggingLevel::ERROR, __LINE__, __PRETTY_FUNCTION__);
                }
            }
        }
    }

    mutable std::mutex m_mtx;              ///< Mutex for thread-safe logging
    std::string m_fileName;                ///< Name of the log file
    std::atomic<bool> m_enableTime;        ///< Flag to enable/disable time-stamping of logs
    std::atomic<bool> m_saveFile;          ///< Flag to enable/disable saving logs to a file
    std::atomic<bool> m_enable;            ///< Flag to enable/disable logging
    std::atomic<LoggingLevel> m_logLevel;  ///< Current logging level
};

#endif
