#include "logger.h"

Logger *Logger::m_instance = Logger::getInstance();

void exit() {
    log() -> Flush();
}

Logger::Logger() {
    LogsPath = QDir::currentPath() + QDir::separator() + "Logs";
    createLogsDirectory();
    enableLogging = true;
    buffer.reserve(FLUSHRATE + 1000);
    atexit(exit);
}

Logger *Logger::getInstance() {
    if (Logger::m_instance == nullptr) Logger::m_instance = new Logger;
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
    } else {
        qWarning() << "Could not create '/Logs' path!";

        return false;
    }
}

void Logger::Fatal(QString msg) {
    errorLog = true;

    msg = getTimeDate() + " FATAL: " + msg + "\n";

    writer(msg);
}

void Logger::Error(QString msg) {
    errorLog = true;

    msg = getTimeDate() + " ERROR: " + msg + "\n";

    writer(msg);
}

void Logger::Warn(QString msg) {
    errorLog = true;

    msg = getTimeDate() + " WARN: " + msg + "\n";

    writer(msg);
}

void Logger::Info(QString msg) {
    errorLog = false;

    msg = getTimeDate() + " INFO: " + msg + "\n";

    writer(msg);
}

void Logger::Debug(QString msg) {
    errorLog = true;

    msg = getTimeDate() + " DEBUG: " + msg + "\n";

    writer(msg);
}

void Logger::Trace(QString msg) {
    errorLog = false;

    msg = getTimeDate() + " TRACE: " + msg + "\n";

    writer(msg);
}

void Logger::Event(QString msg) {
    errorLog = true;

    msg = getTimeDate() + " EVENT: " + msg + "\n";

    writer(msg);
}

void Logger::Flush() {
    QSharedMemory semaphore("flushingInProgress");

    if (semaphore.lock()) {
        Logger::Flush();
    } else {
        Logger::flusher();
    }
}

void Logger::flusher() {
    updateLogFilePath();
    QFile file(Logger::LogFilePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append) && !buffer.isEmpty()) {
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

void Logger::writer(QString data) {
    if (Logger::enableLogging) {
        QSharedMemory semaphore("loggingInProgress");

        if (semaphore.lock()) {
            Logger::writer(data);
        } else {
            buffer.append(data.toLocal8Bit());

            if (buffer.size() + data.size() > FLUSHRATE or errorLog) {
                Logger::Flush();
            }
        }
    }
}
