#ifndef LOGGER_H
#define LOGGER_H

#ifdef log
#undef log
#endif

#define Log Logger::getInstance

#include <QtCore/QString>

/**
 * A class to log messages to a file.
 */
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

    /**
     * Flushes the output stream.
     *
     * @returns None
     */
    void Flush();

    /**
     * Logs a FATAL type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Fatal(QString msg);

    /**
     * Logs an ERROR type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Error(QString msg);

    /**
     * Logs a WARN type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Warn(QString msg);

    /**
     * Logs an INFO type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Info(QString msg);

    /**
     * Logs a DEBUG type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Debug(QString msg);

    /**
     * Logs a TRACE type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Trace(QString msg);

    /**
     * Logs an EVENT type message.
     *
     * @param msg The message to log.
     *
     * @returns None
     */
    void Event(QString msg);

    /**
     * Updates the log file path.
     *
     * @returns None
     */
    void updateLogFilePath();

    /**
     * Returns bool value indicating whether logging is enabled.
     *
     * @returns Bool value indicating whether logging is enabled.
     */
    bool isEnabled() { return Logger::isLoggingEnabled; }

    /**
     * Enables or disables logging.
     *
     * @param enableLogging Whether to enable logging.
     *
     * @returns None
     */
    void enableLogging(bool enableLogging) { Logger::isLoggingEnabled = enableLogging; }

    /**
     * Returns the log directory.
     *
     * @returns The log directory.
     */
    QString getLogDir() { return Logger::logDir; }

    /**
     * Sets the log directory.
     *
     * @param logDir The directory to log to.
     *
     * @returns None
     */
    void setLogDir(const QString &logDir) {
        Logger::logDir = logDir;
        createLogsDirectory();
    }

    /**
     * Returns the name of the log file.
     *
     * @returns The name of the log file.
     */
    QString getLogFileName() { return Logger::logFileName; }

    /**
     * Sets the log file name.
     *
     * @param logFileName The name of the log file.
     *
     * @returns None
     */
    void setLogFileName(const QString &logFileName) { Logger::logFileName = logFileName; }

    /**
     * Returns the flush rate of the logger.
     *
     * @returns The flush rate of the logger.
     */
    int getFlushRate() { return Logger::flushRate; }

    /**
     * Sets the flush rate of the logger.
     *
     * @param flushRate The flush rate of the logger.
     *
     * @returns None
     */
    void setFlushRate(int flushRate) { Logger::flushRate = flushRate; }

   private:
    Logger();

    /**
     * Creates the logs directory if it does not exist.
     *
     * @returns True if the logs directory was created, false otherwise.
     */
    bool createLogsDirectory();

    /**
     * Writes a message to the log file.
     *
     * @param logMsg The message to write.
     * @param logType The type of message to write.
     * @param isErrorLog Whether the message is an error message.
     *
     * @returns None
     */
    void writer(QString logMsg, QString logType, bool isErrorLog);

    bool isLoggingEnabled;
    QString logDir;
    QString logFileName;
    int flushRate;

    QByteArray buffer;
    bool errorLog;
};

#endif  // LOGGER_H