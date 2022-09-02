#include "listener.h"

void Listener::onReceived(QByteArray message) { emit backend->getListenerText(message); }

void Listener::disconnect() {
    // TODO: try catch doesn't handle the nullptr nor does a simple nullptr check
    // That is why i am using a flag as a temporary solution
    if (connected) {
        getSocket()->abort();
        connected = false;
    }

    /* TODO
    create a disconnect function in TcpClient Class:
    disconnect() {
        if (connected) {
            getSocket()->abort();
            connected = false;
        }
    }
    And then create a isConnected function:
    bool isConnected() { return connected; }
    in the attemptConnection function set this flag to true
    After this, use these new functions in listener.cpp and the other clients that may need them
    */
}