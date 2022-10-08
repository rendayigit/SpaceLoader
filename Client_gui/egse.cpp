#include "egse.h"

Egse::Egse(Backend* backend) { this->backend = backend; };

void Egse::onReceived(QByteArray message) {
    backend->egseReplier(message);
    // delete this; // TODO disconnect sometime
}

// TODO - implement
void Egse::buttonCallback(QString buttonId) {
    if (buttonId.contains("STARKIT")) {
        interpret("REMOTE");
        interpret("BYPASS A");
        // TODO ...
    } else if (buttonId.contains("MUB_V1")) {
    } else if (buttonId.contains("Remote")) {
    } else if (buttonId.contains("Local")) {
    } else if (buttonId.contains("Bypass A")) {
    } else if (buttonId.contains("Bypass B")) {
    }
}

// TODO - implement
void Egse::interpret(QString tc) { transmit(tc.toLocal8Bit()); }