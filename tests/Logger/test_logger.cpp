#include <gtest/gtest.h>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDebug>
#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>

#include "../../lib/Logger/logger.h"
#include "../Test_common.h"

using namespace std;

QString logString = "Testing Logger";

void deleteLogFile() {
    QFile file(log()->LogFilePath);
    if (file.exists()) {
        file.remove();
    }
}

TEST(Logger, Write) {
    deleteLogFile();
    log()->Info(logString);
    log()->Flush();
}

TEST(Logger, Read) {
    QFile file(log()->LogFilePath);

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
        GTEST_FAIL() << "Log file not found: " + log()->LogFilePath.toStdString();
    }
}

TEST(Logger, Benchmark) {
    TEST_BENCHMARK("Logger", "loggingManualBenchmark", log()->Info("benchmark log"));
    log()->Flush();
}

void writeThreadFirst() {
    for (int i = 0; i < 100; i++) {
        log()->Warn("Concurrent Write Test Thread 1 log " + QString::number(i));
    }
}

void writeThreadSecond() {
    for (int i = 100; i < 200; i++) {
        log()->Warn("Concurrent Write Test Thread 2 log " + QString::number(i));
    }
}

TEST(Logger, ThreadSafety) {
    std::thread first(writeThreadFirst);
    std::thread second(writeThreadSecond);

    int logCount = 200;

    bool arr[logCount];
    bool allTrue = true;

    for (bool& i : arr) {
        i = false;
    }

    QFile file(log()->LogFilePath);

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        first.join();
        second.join();
    } else {
        GTEST_FAIL() << "Log file not found: " + log()->LogFilePath.toStdString();
    }

    while (not file.atEnd()) {
        QString line(file.readLine());
        if (line.trimmed().contains("Thread")) {
            QRegExp rx("^.*[0-9]+$");
            if (rx.indexIn(line) > -1) {
                arr[rx.cap(0).toInt()] = true;
            }
        }
    }

    for (bool i : arr) {
        if (not i) {
            allTrue = false;
        }
    }

    while (first.joinable() || second.joinable());

    first.detach();
    second.detach();

    EXPECT_TRUE(allTrue);

    file.close();
}
