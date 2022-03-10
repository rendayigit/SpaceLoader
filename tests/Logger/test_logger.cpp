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
    int logCount = 200;

    std::vector<bool> arr;
    bool allTrue = true;

    for (int i = 0; i < arr.size(); i++) {
        arr.at(i) = false;
    }

    // deleteLogFile();
    QFile file(log()->LogFilePath);

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        std::thread first(writeThreadFirst);
        std::thread second(writeThreadSecond);
        first.join();
        second.join();
    } else {
        GTEST_FAIL() << "Log file not found: " + log()->LogFilePath.toStdString();
    }

    file.close();

    QFile file2(log()->LogFilePath);
    file2.open(QIODevice::ReadOnly);
    if (!file2.isOpen()) return;

    QTextStream stream(&file2);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine()) {
        if (line.trimmed().contains("Thread")) {
            QRegExp rx("\'s(\'w+)$");
            if (rx.indexIn(line) > -1) {
                int idx = rx.cap(0).toInt();
                qDebug() << idx;
                arr.at(idx) = true;
            }
        }
    };

    std::vector<int> missing = {};

    for (int i = 0; i < arr.size(); i++) {
        if (not arr.at(i)) {
            allTrue = false;
            missing.push_back(i);
        }
    }

    EXPECT_TRUE(allTrue);

    file2.close();
}
