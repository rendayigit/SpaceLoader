#ifndef TESTCOMMON_H
#define TESTCOMMON_H
#include <gtest/gtest.h>

#include <QtCore/QtCore>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>

#define TEST_COUT(message) ((std::cerr << "[   PRINT  ] " << (message) << std::endl))

#define GET_TIME()                                                \
    (double)std::chrono::duration_cast<std::chrono::nanoseconds>( \
        std::chrono::system_clock::now().time_since_epoch())      \
        .count();

#define GET_ELAPSED_TIME(timeStart, timeEnd) ((double)timeEnd - (double)timeStart)

#define TEST_BENCHMARK(test_name, test, func)                                       \
    uint64_t timeStart = GET_TIME();                                                \
    int i = 0, itr = 1000;                                                          \
    for (; i < itr; i++) {                                                          \
        func;                                                                       \
    }                                                                               \
    uint64_t timeEnd = GET_TIME();                                                  \
    double timeResult = GET_ELAPSED_TIME(timeStart, timeEnd);                       \
    std::string res = "";                                                           \
    if (timeResult >= 1000000000) {                                                 \
        res = "Average Time : " + std::to_string(timeResult * pow(10, -9)) + " sn"; \
        TEST_COUT(res);                                                             \
    } else if (timeResult >= 1000000) {                                             \
        res = "Average Time : " + std::to_string(timeResult * pow(10, -6)) + " ms"; \
        TEST_COUT(res);                                                             \
    } else if (timeResult >= 1000) {                                                \
        res = "Average Time : " + std::to_string(timeResult * pow(10, -3)) + " μs"; \
        TEST_COUT(res);                                                             \
    } else {                                                                        \
        res = "Average Time : " + std::to_string(timeResult) + " nsecs";            \
        TEST_COUT(res);                                                             \
    }

#endif  // TESTCOMMON_H