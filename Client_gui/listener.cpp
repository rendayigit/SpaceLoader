#include "listener.h"

void Listener::onReceived(QByteArray message) { emit backend->getListenerText(message); }

void Listener::onDisconnected() {}

void Listener::disconnect() {
    if (getSocket() != nullptr) {
        getSocket()->abort();
    }
}