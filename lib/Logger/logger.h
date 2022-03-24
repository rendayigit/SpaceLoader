#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSharedMemory>
#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <filesystem>

using std::filesystem::current_path;

#ifdef log
#undef log
#endif

#define log Logger::getInstance

class Logger {
   public:
    static Logger *getInstance();
    static QString getTimeDate();
    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;
    void Flush();
    void Fatal(QString msg);
    void Error(QString msg);
    void Warn(QString msg);
    void Info(QString msg);
    void Debug(QString msg);
    void Trace(QString msg);
    void Event(QString msg);

    bool enableLogging;
    QString LogsPath;
    QString LogFilePath;
    int FLUSHRATE = 100000;

    void updateLogFilePath();

   private:
    Logger();
    ~Logger();
    bool createLogsDirectory();
    void writer(QString data);
    void flusher();

    static Logger *m_instance;
    QByteArray buffer;
    bool errorLog;
};

#endif  // YAML_H