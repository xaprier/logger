#include "Logger.hpp"

#include <chrono>
#include <mutex>
#include <optional>
#include <string>

#include "LoggingLevel.hpp"

Logger::Logger(LoggingLevel level, std::string fileName, bool saveToFile, bool enableTimer, bool enable)
    : m_logLevel{level}, m_fileName{fileName}, m_saveFile{saveToFile}, m_enableTime{enableTimer}, m_enable{enable} {
}

Logger::~Logger() {
    if (!this->m_enable.load()) return;
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - logger_start_time);
    std::lock_guard<std::mutex> lock(m_mtx);
    std::ofstream file(m_fileName, std::ios::app);
    if (file.is_open()) {
        file << "[Logger] Shutting down. All logs flushed: " << duration.count() << "ms" << std::endl;
    }
    std::cout << "[Logger] Destructor executed: " << duration.count() << "ms\n";
}

void Logger::EnableLog() {
    this->m_enable.store(true);
}
void Logger::DisableLog() {
    this->m_enable.store(false);
}
void Logger::SetLog(bool save) {
    this->m_enable.store(save);
}

void Logger::EnableFile() {
    this->m_saveFile.store(true);
}

void Logger::DisableFile() {
    this->m_saveFile.store(false);
}

void Logger::SetFile(bool save) {
    this->m_saveFile.store(save);
}

void Logger::EnableTimer() {
    this->m_enableTime.store(true);
}

void Logger::DisableTimer() {
    this->m_enableTime.store(false);
}

void Logger::SetTimer(bool timingOn) {
    this->m_enableTime.store(timingOn);
}

void Logger::SetLogLevel(LoggingLevel level) {
    m_logLevel.store(level);
}

void Logger::SetLogFileName(const std::string &fileName) {
    this->m_fileName = fileName;
}

bool Logger::IsLogEnabled() const {
    return m_enable.load();
}

bool Logger::IsTimerEnabled() const {
    return m_enableTime.load();
}

bool Logger::IsFileSaveEnabled() const {
    return m_saveFile.load();
}

LoggingLevel Logger::GetLevel() const {
    return m_logLevel.load();
}

std::string Logger::GetFileName() const {
    return m_fileName;
}

std::string Logger::_LogTime() const {
    auto duration = std::chrono::steady_clock::now() - logger_start_time;
    long double execution;
    std::string unit;
    if (duration < std::chrono::microseconds(1)) {
        execution = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        unit = "ns";
    } else if (duration < std::chrono::milliseconds(1)) {
        execution = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        unit = "μs";
    } else if (duration < std::chrono::seconds(1)) {
        execution = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        unit = "ms";
    } else {
        execution = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        execution /= 1000.0;
        unit = "s";
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << execution;  // Set precision to 3 decimal places

    std::string output = "[" + oss.str() + unit + "]";
    return output;
}
