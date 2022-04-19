#include <gtest/gtest.h>

#include "../../Commands/hello/hello.h"
#include "../../common.h"
#include "../Test_common.h"

TEST(Commands, hello) {
    QTcpSocket *testSocket{};

    runDynamicCmd(testSocket, "hello");

    /* TODO: Read logs and see if string ("Ran Hello !!") exists */
}