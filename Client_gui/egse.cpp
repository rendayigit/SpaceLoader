#include "egse.h"

Egse::Egse(Backend* backend) { this->backend = backend; };

// TODO - implement
void Egse::onReceived(QByteArray message) {
    if (message == "ACK") {
        backend->egseReply();
    } else {

    }
}

// TODO - implement
void Egse::sendTc(QByteArray tc) {
    transmit(tc);
}