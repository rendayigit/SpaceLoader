#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>

/**
 * A class for TCP Client operations.
 */
class TCPClient : public QObject {
    Q_OBJECT
   protected:
    /**
     * @brief TCPClient - Create a TCP client.
     * @param parent
     */
    explicit TCPClient(QObject *parent = 0);

    /**
     * @brief onReceived - This virtual function is automatically called
     * when data from the server is received.
     * @param message - message received from the server in raw format.
     */
    virtual void onReceived(QByteArray message) = 0;

    /**
     * @brief onDisconnected - This virtual function is automatically called
     * when the client is disconnected from the server. Please note that
     * sometimes the client may need to attemp to contact the server to realize
     * the connection has dropped.
     */
    virtual void onDisconnected() = 0;

    /**
     * @brief Get the Socket object of the client.
     *
     * @return QTcpSocket*
     */
    QTcpSocket *getSocket() const { return socket; }

   public slots:
    /**
     * @brief attemptConnection - Establishes a connection to a server.
     * @param serverIP - IP of the server.
     * @param serverPort - Port number.
     * @return true - success, false - fail
     */
    bool attemptConnection(QString serverIP, qint32 serverPort);

    /**
     * @brief transmit - Transmit raw data to connected server.
     * @param data - Raw data to transmit
     */
    void transmit(QByteArray data);

    /**
     * Waits for data to be available on the input queue.
     *
     * @param timeout The number of milliseconds before timeout.
     *
     * @returns None
     */
    void waitForData(qint32 timeout = 1000);

   private slots:
    /**
     * Forwards received data from the serial port to onReceived().
     *
     * @returns None
     */
    void onReadyRead();

    /**
     * Disconnects from the server in any case of a server connection error.
     *
     * @param socketError The error code for the socket.
     *
     * @returns None
     */
    void onError(QAbstractSocket::SocketError socketError);

   private:
    QTcpSocket *socket;
};

#endif  // TCPCLIENT_H
