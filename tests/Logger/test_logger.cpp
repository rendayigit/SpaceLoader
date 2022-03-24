#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDebug>
#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>

#include "../../lib/Logger/logger.h"
#include "../Test_common.h"

using namespace std;

QString logString = "Testing Logger";
QByteArray buffer;

TEST(Logger, Write) {
    Log()->Info(logString);
    Log()->Flush();
}

TEST(Logger, Read) {
    QFile file(Log()->LogFilePath);

    EXPECT_TRUE(file.exists());
    EXPECT_TRUE(file.open(QIODevice::ReadOnly));

    bool logStringFound = false;

    while (not file.atEnd()) {
        QString line(file.readLine());

        if (line.trimmed().contains(logString)) {
            logStringFound = true;
        }
    }

    EXPECT_TRUE(logStringFound);
    file.close();
}

TEST(Logger, Benchmark) {
    TEST_BENCHMARK("Logger", "loggingManualBenchmark", Log()->Info("benchmark log"));
    Log()->Flush();
}


void writeThreadFirst() {
    for (int i = 0; i < 100; i++)
        Log()->Warn("Concurrent Write Test Thread 1 log " + QString::number(i));
}
void writeThreadSecond() {
    for (int i = 100; i < 201; i++)
        Log()->Warn("Concurrent Write Test Thread 2 log " + QString::number(i));
}

TEST(Logger, ThreadSafety) {
    if (not QString(Log()->LogFilePath).isNull())
     QFile::remove(Log()->LogFilePath);

    std::thread first(writeThreadFirst);
    std::thread second(writeThreadSecond);

    first.join();
    second.join();

    bool arr[201], allTrue = true;
    for (int i = 0; i < 201; i++) arr[i] = false;

    QFile file(Log()->LogFilePath);

    EXPECT_TRUE(file.exists());
    EXPECT_TRUE(file.open(QIODevice::ReadOnly));

    for (int i = 0; i < 201; i++) arr[i] = false;

    while (not file.atEnd()) {
        QString line(file.readLine());
        if (line.trimmed().contains("Thread 1")) {
            arr[line.right(3).toInt()] = true;
        }
        if (line.trimmed().contains("Thread 2")) {
            arr[line.right(4).toInt()] = true;
        }
    }

    for (int i = 0; i < 201; i++) {
        if (arr[i] == 0) allTrue = false;
    }

    // TODO uncomment and fix this
    // EXPECT_TRUE(allTrue);

    file.close();

    // TODO - read every number twice from 0 to 99
}
