#include "egse.h"

Egse::Egse(Backend* backend) { this->backend = backend; };

void Egse::onReceived(QByteArray message) {
    backend->egseReplier(message);
    delete this;
}

// TODO - implement
void Egse::sendTc(QByteArray tc) { transmit(tc); }