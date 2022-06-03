#ifndef TCPUNITTESTS_H
#define TCPUNITTESTS_H

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QObject>
#include <QtTest/QTest>
#include "../Test_common.h"
#include "testClient.h"
#include "testServer.h"

class tcpUnitTests : public QObject {
    Q_OBJECT
   public:
    explicit tcpUnitTests(QObject *parent = nullptr);

   private slots:
    void startServer();
    void startClients();
    void transmitFromClient();
    void ReceiveFromServer();
    void broadcastFromServer();
    void ReceiveFromClients();

   private:
    TestServer *server;
    TestClient *client1, *client2, *client3, *client4;
    QString testString1 = "TEST STRING 1";
    QString testString2 = "TEST STRING 2";
    bool allClientsInitiated = false;
};

#endif  // TCPUNITTESTS_H