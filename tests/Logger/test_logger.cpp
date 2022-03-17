#include <gtest/gtest.h>
#include <unistd.h>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDebug>
#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <atomic>
#include <cmath>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <thread>
std::condition_variable condition_;
std::mutex mutex_;
bool flag = false;

#include "../../lib/Logger/logger.h"
#include "../Test_common.h"

using namespace std;

QString logString = "Testing Logger";

void deleteLogFile() {
    QFile file(log()->LogFilePath);
    if (file.exists()) {
        file.resize(0);
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
    unique_lock<mutex> lock(mutex_);
    for (int i = 0; i < 100; i++) {
        condition_.wait(lock, [] { return flag ? true : false; });
        log()->Warn("Concurrent Write Test Thread 1 log " + QString::number(i));
        flag = !flag;
        condition_.notify_one();
    }
}

void writeThreadSecond() {
    unique_lock<mutex> lock(mutex_);
    for (int i = 100; i < 200; i++) {
        condition_.wait(lock, [] { return !flag ? true : false; });
        
        log()->Warn("Concurrent Write Test Thread 2 log " + QString::number(i));
        flag = !flag;
        condition_.notify_one();
    }
}

TEST(Logger, ThreadSafety) {
    deleteLogFile();
    bool arr[200];
    bool allTrue = true;

    for (int i = 0; i < 200; i++) {
        arr[i] = false;
    }

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

    QString lines;

    QRegExp rx;
    rx.setPattern("log (.*)");
    while (!file.atEnd()) {
        lines = file.readLine();
        if (lines.contains("Thread")) {
            if (rx.indexIn(lines) != -1) {
                int idx = rx.cap(1).toInt();
                //  QString s = QString::number(idx);// for printing the values with log
                //  log()->Info(s);
                arr[idx] = true;
            }
        }
    }

    for (int i = 0; i < 200; i++) {
        if (not arr[i]) {
            allTrue = false;
            break;
        }
    }

    EXPECT_TRUE(allTrue);
}
