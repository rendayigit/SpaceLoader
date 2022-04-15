#include <QtConcurrent/QtConcurrent>
#include <QtCore/QObject>
#include "../Test_common.h"
#include "testClient.h"
#include "testServer.h"
#ifndef TCPUNITTESTS_H
#define TCPUNITTESTS_H
#endif  // TCPUNITTESTS_H

using namespace std;

TestServer *server;
TestClient *client1, *client2, *client3, *client4;
QString testString1 = "TEST STRING 1";
QString testString2 = "TEST STRING 2";
bool allClientsInitiated = false;

TEST(tcp_test, startServer) {
    server = new TestServer();
    qint32 serverPort = 1234;
    server->startServer(serverPort);

}

TEST(tcp_test, startClients) {
    QFuture<void> future = QtConcurrent::run([=]() {
        client1 = new TestClient();
        client1->attemptConnection("127.0.0.1", 1234);

        client2 = new TestClient();
        client2->attemptConnection("127.0.0.1", 1234);

        client3 = new TestClient();
        client3->attemptConnection("127.0.0.1", 1234);

        client4 = new TestClient();
        client4->attemptConnection("127.0.0.1", 1234);

        allClientsInitiated = true;
    });
}
TEST(tcp_test, transmitFromClient) {
    while (allClientsInitiated != true) QThread::msleep(10);
    client2->sendCommand(testString1.toLocal8Bit());
}
TEST(tcp_test, ReceiveFromServer) {
    // TODO uncomment and fix this
    // EXPECT_TRUE(server->receivedString == testString1)
    //     << "String received from the server and string transmitted from one of the clients are not "
    //        "identical!";
}
TEST(tcp_test, broadcastFromServer) { server->broadcast(testString2.toLocal8Bit()); }
TEST(tcp_test, ReceiveFromClients) {
    client1->waitForData();
    client2->waitForData();
    client3->waitForData();
    client4->waitForData();

    EXPECT_TRUE(QString::compare(client1->receivedString, testString2))
        << "String received from client1 and string transmitted from the server are not identical!";
        qDebug()<<client1->receivedString<<": received";
    EXPECT_TRUE(QString::compare(client2->receivedString, testString2))
        << "String received from client2 and string transmitted from the server are not identical!";
    EXPECT_TRUE(QString::compare(client3->receivedString, testString2))
        << "String received from client3 and string transmitted from the server are not identical!";
    EXPECT_TRUE(QString::compare(client4->receivedString, testString2))
        << "String received from client4 and string transmitted from the server are not identical!";
}