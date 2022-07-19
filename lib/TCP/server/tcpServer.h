#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtCore/QDataStream>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QtCore>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

/**
 * A class for TCP Server operations.
 */
class TCPServer : public QObject {
    Q_OBJECT
   public:
    /**
     * @brief TCPServer - Create a TCP server.
     */
    explicit TCPServer(QObject *parent = 0);

    /**
     * @brief startServer - Start listening to given port.
     * @param portNumber - Port number for the server to listen to.
     */
    void startServer(qint32 portNumber);

    /**
     * @brief onReceived - This virtual function is automatically called
     * when data from the a client is received.
     * @param sender - Sender socket.
     * @param message - message received from the client in raw format.
     */
    virtual void onReceived(QTcpSocket *sender, QByteArray message) = 0;

    /**
     * @brief clientDisconnected - This virtual function is automatically called
     * when a client is disconnected from the server.
     * @param clientSocket - client socket.
     */
    virtual void clientDisconnected(QTcpSocket *clientSocket) = 0;

    /**
     * @brief broadcast - Broadcast a message to all connected clients.
     * @param message - Message to broadcast.
     */
    void broadcast(QByteArray message);

    /**
     * @brief transmit - Send a message to given client.
     *
     * @param client - Client socket.
     * @param message - Message to send.
     */
    static void transmit(QTcpSocket *client, QByteArray message);

   public slots:

    /**
     * A callback function that is called when a new connection is established.
     *
     * @returns None
     */
    void onNewConnection();

    /**
     * A callback function that is invoked when the input tensor is disconnected.
     *
     * @returns None
     */
    void onDisconnected();

    /**
     * Forwards received data from the serial port to onReceived().
     *
     * @returns None
     */
    void onReadyRead();

   private:
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket *> socketList;
};

#endif  // TCPSERVER_H
