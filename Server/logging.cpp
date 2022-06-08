#include "logging.h"

#include "../common.h"
#include "../lib/Logger/logger.h"

QString Logging::getLogData(QString fileName) {
    if (not fileName.contains(".log")) {
        fileName += ".log";
    }

    QString logFile = Log().getLogDir() + fileName;

    QFile file(logFile);

    if (!file.exists()) {
        QString warning = logFile + " not found!";
        Log().Warn(warning);
        return warning;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        QString warning = "Cannot read " + logFile + " !";
        Log().Warn(warning);
        return warning;
    }

    QString lines = "Reading " + fileName + ":\n";

    while (!file.atEnd()) {
        lines += file.readLine();
    }
    file.close();

    return lines;
}

QString Logging::getLogFileNames() {
    QDir directory(Log().getLogDir());
    QStringList logFiles = directory.entryList(QStringList() << "*.log", QDir::Files);
    logFiles.prepend("Server Logs:");
    return logFiles.join('\n');
}