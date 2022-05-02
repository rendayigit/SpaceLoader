#include "listener.h"

void Listener::onReceived(QByteArray message) { emit backend->getListenerText(message); }

void Listener::onDisconnected() {}

void Listener::disconnect() {
    // TODO: try catch doesn't handle the nullptr nor does a simple nullptr check
    // That is why i am using a flag as a temporary solution
    if (connected) {
        getSocket()->abort();
        connected = false;
    }
}