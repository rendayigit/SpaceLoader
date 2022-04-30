#ifndef LOGGER_H
#define LOGGER_H

#ifdef log
#undef log
#endif

#define Log Logger::getInstance

#include <QtCore/QString>

class Logger {
   public:
    Logger(const Logger &) = delete;
    Logger &operator=(Logger const &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(Logger &&) = delete;
    ~Logger() = default;

    static auto &getInstance() {
        static Logger instance;
        return instance;
    }

    void Flush();
    void Fatal(QString msg);
    void Error(QString msg);
    void Warn(QString msg);
    void Info(QString msg);
    void Debug(QString msg);
    void Trace(QString msg);
    void Event(QString msg);

    void updateLogFilePath();

    bool isEnabled() { return Logger::isLoggingEnabled; }
    void enableLogging(bool enableLogging) { Logger::isLoggingEnabled = enableLogging; }

    QString getLogDir() { return Logger::logDir; }
    void setLogDir(const QString &logDir) { Logger::logDir = logDir; }

    QString getLogFileName() { return Logger::logFileName; }
    void setLogFileName(const QString &logFileName) { Logger::logFileName = logFileName; }

    int getFlushRate() { return Logger::flushRate; }
    void setFlushRate(int flushRate) { Logger::flushRate = flushRate; }

   private:
    Logger();

    bool createLogsDirectory();
    void writer(QString logMsg, QString logType, bool isErrorLog);

    bool isLoggingEnabled;
    QString logDir;
    QString logFileName;
    int flushRate;

    QByteArray buffer;
    bool errorLog;
};

#endif  // LOGGER_H