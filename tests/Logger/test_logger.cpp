#include <gtest/gtest.h>

#include "../../lib/Logger/logger.h"
#include "../Test_common.h"

QString logString = "Testing Logger";

void deleteLogFile() {
    QFile file(Log()->getLogDir() + Log()->getLogFileName());
    if (file.exists()) {
        file.resize(0);
    }
}

TEST(Logger, Write) {
    deleteLogFile();
    Log()->Info(logString);
    Log()->Flush();
}

TEST(Logger, Read) {
    QFile file(Log()->getLogDir() + Log()->getLogFileName());

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);

        bool logStringFound = false;

        while (not file.atEnd()) {
            QString line(file.readLine());

            if (line.trimmed().contains(logString)) {
                logStringFound = true;
            }
        }

        EXPECT_TRUE(logStringFound);
        file.close();
        deleteLogFile();
    } else {
        GTEST_FAIL() << "Log file not found: " +
                            (Log()->getLogDir() + Log()->getLogFileName()).toStdString();
    }
}

TEST(Logger, Benchmark) {
    TEST_BENCHMARK("Logger", "loggingManualBenchmark", Log()->Info("benchmark log"));
    Log()->Flush();
}

void writeThreadFirst() {
    for (int i = 0; i < 100; i++) {
        Log()->Warn("Concurrent Write Test Thread 1 log " + QString::number(i));
    }
}

void writeThreadSecond() {
    for (int i = 100; i < 200; i++) {
        Log()->Warn("Concurrent Write Test Thread 2 log " + QString::number(i));
    }
}

TEST(Logger, ThreadSafety) {
    deleteLogFile();
    std::array<bool, 200> logsArray{};
    bool allLogsFound = true;
    int logCount = 0;

    for (int i = 0; i < 200; i++) {
        logsArray.at(i) = false;
    }

    QFile file(Log()->getLogDir() + Log()->getLogFileName());

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        std::thread first(writeThreadFirst);
        std::thread second(writeThreadSecond);
        first.join();
        second.join();
    } else {
        GTEST_FAIL() << "Log file not found: " +
                            (Log()->getLogDir() + Log()->getLogFileName()).toStdString();
    }

    QRegExp rx;
    rx.setPattern("log (.*)");
    QString lines;
    while (!file.atEnd()) {
        lines = file.readLine();
        if (lines.contains("Thread")) {
            logCount++;
            if (rx.indexIn(lines) != -1) {
                int idx = rx.cap(1).toInt();
                logsArray.at(idx) = true;
            }
        }
    }

    EXPECT_EQ(logCount, 200);

    for (bool i : logsArray) {
        if (not i) {
            allLogsFound = false;
            break;
        }
    }

    EXPECT_TRUE(allLogsFound);
}
