#include <gtest/gtest.h>

#include "../../Commands/hello/hello.h"
#include "../../common.h"
#include "../../lib/Logger/logger.h"
#include "../Test_common.h"

TEST(Commands, hello) {
    QTcpSocket *testSocket{};

    CmdPlugin *testPlugin = new CmdPlugin();
    testPlugin->run(testSocket, "hello");
    TEST_COUT(testPlugin->getDescription().toStdString());

    /* Read logs and see if string ("Ran Hello !!") exists */
    QFile file(Log().getLogDir() + Log().getLogFileName());

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
    } else {
        GTEST_FAIL() << "Log file not found: " +
                            (Log().getLogDir() + Log().getLogFileName()).toStdString();
    }

    bool pass = false;
    QString line;
    while (!file.atEnd()) {
        if (file.readLine().contains("Ran Hello !!")) {
            pass = true;
        }
    }

    EXPECT_TRUE(pass);
}