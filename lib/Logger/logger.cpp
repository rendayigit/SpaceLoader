#include "logger.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSharedMemory>
#include <QtCore/QStandardPaths>
#include <filesystem>
#include <mutex>

using std::filesystem::current_path;

std::mutex fileMutex;
std::mutex bufferMutex;

Logger *Logger::m_instance = Logger::getInstance();

void exit() { Log()->Flush(); }

Logger::Logger()
    : isLoggingEnabled(true),
      logDir(QDir::currentPath() + QDir::separator() + "Logs" + QDir::separator()),
      logFileName(""),
      flushRate(100000),
      buffer({}),
      errorLog(false) {
    createLogsDirectory();
    buffer.reserve(flushRate + 1000);
    atexit(exit);
}

Logger *Logger::getInstance() {
    if (m_instance == nullptr) {
        m_instance = new Logger;
    }
    return m_instance;
}

/// Create /Logs directory if it does not already exist
bool Logger::createLogsDirectory() {
    QDir dir(logDir);

    if (dir.exists()) {
        return true;
    }

    if (dir.mkpath(logDir)) {
        return true;
    }

    qWarning() << "Could not create '/Logs' path!";
    return false;
}

void Logger::Fatal(QString msg) { writer(msg, "FATAL", true); }

void Logger::Error(QString msg) { writer(msg, "ERROR", true); }

void Logger::Warn(QString msg) { writer(msg, "WARN", true); }

void Logger::Info(QString msg) { writer(msg, "INFO", false); }

void Logger::Debug(QString msg) { writer(msg, "DEBUG", true); }

void Logger::Trace(QString msg) { writer(msg, "TRACE", false); }

void Logger::Event(QString msg) { writer(msg, "EVENT", true); }

void Logger::Flush() {
    std::unique_lock<std::mutex> lock(fileMutex);

    updateLogFilePath();
    QFile file(logDir + logFileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append) and not buffer.isNull()) {
        file.write(buffer);
        file.close();
        buffer.clear();
    }
}

// TODO call this func only at midnight
void Logger::updateLogFilePath() {
    QDate currentDate = QDateTime::currentDateTime().date();
    logFileName = QString::number(currentDate.day()) + "." + QString::number(currentDate.month()) +
                  "." + QString::number(currentDate.year()) + ".log";
}

void Logger::writer(QString logMsg, QString logType, bool isErrorLog) {
    logMsg = QDateTime::currentDateTime().toString() + " " + logType + " " + logMsg + "\n";

    if (isLoggingEnabled) {
        std::unique_lock<std::mutex> lock(bufferMutex);
        buffer.append(logMsg.toLocal8Bit());

        if (buffer.size() + logMsg.size() > flushRate or isErrorLog) {
            Flush();
        }
    }
}