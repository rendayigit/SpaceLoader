#ifndef LOGGING_H
#define LOGGING_H

#include "QtCore/QtCore"

class Logging {
   public:
    Logging() = default;
    Logging(const Logging &) = delete;
    Logging &operator=(const Logging &) = delete;
    Logging(Logging &&) = delete;
    Logging &operator=(Logging &&) = delete;
    ~Logging() = default;

    static QString getLogData(QString fileName);
    static QString getLogFileNames();
};

#endif  // LOGGING_H
