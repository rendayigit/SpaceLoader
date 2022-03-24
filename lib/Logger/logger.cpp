#include "logger.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSharedMemory>
#include <QtCore/QStandardPaths>

using std::filesystem::current_path;

std::mutex fileMutex;
std::mutex bufferMutex;

Logger *Logger::m_instance = Logger::getInstance();

void exit() {
    Log() -> Flush();
}

Logger::Logger() {
    LogsPath = QDir::currentPath() + QDir::separator() + "Logs";
    createLogsDirectory();
    enableLogging = true;
    buffer.reserve(FLUSHRATE + 1000);
    atexit(exit);
}

Logger *Logger::getInstance() {
    if (Logger::m_instance == nullptr) {
        Logger::m_instance = new Logger;
    }
    return Logger::m_instance;
}

QString Logger::getTimeDate() { return QDateTime::currentDateTime().toString(); }

/// Create /Logs directory if it does not already exist
bool Logger::createLogsDirectory() {
    QDir dir(Logger::LogsPath);

    if (dir.exists()) {
        return true;
    }

    if (dir.mkpath(Logger::LogsPath)) {
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
    Logger::flusher();
}

void Logger::flusher() {
    updateLogFilePath();
    QFile file(Logger::LogFilePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append) and not buffer.isEmpty()) {
        file.write(buffer);
        buffer.clear();
        file.close();
    }
}

// TODO call this func only at midnight
void Logger::updateLogFilePath() {
    QDate currentDate = QDateTime::currentDateTime().date();
    LogFilePath = Logger::LogsPath + QDir::separator() + QString::number(currentDate.day()) + "." +
                  QString::number(currentDate.month()) + "." + QString::number(currentDate.year()) +
                  ".log";
}

void Logger::writer(QString data, QString type, bool error) {
    errorLog = error;
    data = getTimeDate() + " " + type + " " + data + "\n";

    if (Logger::enableLogging) {
        std::unique_lock<std::mutex> lock(bufferMutex);
        buffer.append(data.toLocal8Bit());

        if (buffer.size() + data.size() > FLUSHRATE or errorLog) {
            Logger::Flush();
        }
    }
}