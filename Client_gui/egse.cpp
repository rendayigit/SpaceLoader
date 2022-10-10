#include "egse.h"
#include <QtCore/QThread>

Egse::Egse(Backend* backend) { this->backend = backend; };

void Egse::onReceived(QByteArray message) {
    backend->egseReplier(message);
    // delete this; // TODO disconnect sometime
}

void Egse::buttonCallback(QString buttonId) {
    if (not connected) {
        return;
    }

    if (buttonId.contains("STARKIT")) {
        interpret("77 0C C0 00 00 0E 54 52 41 4E 53 46 45 52 20 52 45 4D 4F 54 45");  // TRANSFER REMOTE
        interpret("77 0C C0 00 00 0C 53 54 41 52 54 20 2D 4F 6E 6C 69 6E 65");  // START -Online
        interpret("77 0C C0 00 00 0B 54 6F 67 67 6C 65 54 4D 20 4F 66 66");     // ToggleTM Off
        interpret("77 0C C0 00 00 0A 54 6F 67 67 6C 65 54 4D 20 4F 6E");        // ToggleTM On
        interpret("77 0C C0 00 00 0B 54 6F 67 67 6C 65 54 43 20 4F 66 66");     // ToggleTC Off
        interpret("77 0C C0 00 00 0A 54 6F 67 67 6C 65 54 43 20 4F 6E");        // ToggleTC On
        interpret("77 0C C0 00 00 15 49 70 75 43 6D 64 20 74 63 67 2E 70 6C 6F 70 5F 6D 6F 64 65 20 32");  // IpuCmd tcg.plop_mode 2
        interpret("77 0C C0 00 00 0A 53 65 74 54 4D 6D 6F 64 65 20 31");  // SetTMmode 1

        interpret("77 0C C0 00 00 0E 49 70 75 53 65 74 54 63 67 44 65 73 74 20 34");  // IpuSetTcgDest 4
        interpret("77 0C C0 00 00 10 49 70 75 53 65 74 54 6D 61 53 6F 75 72 63 65 20 33");  // IpuSetTmaSource 3
    } else if (buttonId.contains("MUB_V1")) {
        interpret("77 0C C0 00 00 0E 54 52 41 4E 53 46 45 52 20 52 45 4D 4F 54 45");  // TRANSFER REMOTE
        interpret("77 0C C0 00 00 0C 53 54 41 52 54 20 2D 4F 6E 6C 69 6E 65");  // START -Online
        interpret("77 0C C0 00 00 0B 54 6F 67 67 6C 65 54 4D 20 4F 66 66");     // ToggleTM Off
        interpret("77 0C C0 00 00 0A 54 6F 67 67 6C 65 54 4D 20 4F 6E");        // ToggleTM On
        interpret("77 0C C0 00 00 0B 54 6F 67 67 6C 65 54 43 20 4F 66 66");     // ToggleTC Off
        interpret("77 0C C0 00 00 0A 54 6F 67 67 6C 65 54 43 20 4F 6E");        // ToggleTC On
        interpret("77 0C C0 00 00 15 49 70 75 43 6D 64 20 74 63 67 2E 70 6C 6F 70 5F 6D 6F 64 65 20 32");  // IpuCmd tcg.plop_mode 2
        interpret("77 0C C0 00 00 0A 53 65 74 54 4D 6D 6F 64 65 20 31");  // SetTMmode 1

        interpret("77 0C C0 00 00 0E 49 70 75 53 65 74 54 63 67 44 65 73 74 20 32");  // IpuSetTcgDest 2
        interpret("77 0C C0 00 00 10 49 70 75 53 65 74 54 6D 61 53 6F 75 72 63 65 20 32");  // IpuSetTmaSource 2
    } else if (buttonId.contains("Remote")) {
        interpret("77 0C C0 00 00 0E 54 52 41 4E 53 46 45 52 20 52 45 4D 4F 54 45");  // TRANSFER REMOTE
    } else if (buttonId.contains("Local")) {
        interpret("77 0C C0 00 00 0D 54 52 41 4E 53 46 45 52 20 4C 4F 43 41 4C"); // TRANSFER LOCAL
    } else if (buttonId.contains("TC DEST B")) {
        interpret("77 0C C0 00 00 0E 49 70 75 53 65 74 54 63 67 44 65 73 74 20 32");  // IpuSetTcgDest 2
    } else if (buttonId.contains("TM SOURCE B")) {
        interpret("77 0C C0 00 00 10 49 70 75 53 65 74 54 6D 61 53 6F 75 72 63 65 20 32");  // IpuSetTmaSource 2
    } else if (buttonId.contains("TC DEST A")) {
        interpret("77 0C C0 00 00 0E 49 70 75 53 65 74 54 63 67 44 65 73 74 20 34");  // IpuSetTcgDest 4
    } else if (buttonId.contains("TM SOURCE A")) {
        interpret("77 0C C0 00 00 10 49 70 75 53 65 74 54 6D 61 53 6F 75 72 63 65 20 33");  // IpuSetTmaSource 3
    } else if (buttonId.contains("TM CHAIN OFF")) {
        interpret("77 0C C0 00 00 0B 54 6F 67 67 6C 65 54 4D 20 4F 66 66");     // ToggleTM Off
    } else if (buttonId.contains("TM CHAIN ON")) {
        interpret("77 0C C0 00 00 0A 54 6F 67 67 6C 65 54 4D 20 4F 6E");        // ToggleTM On
    } else if (buttonId.contains("TC CHAIN OFF")) {
        interpret("77 0C C0 00 00 0B 54 6F 67 67 6C 65 54 43 20 4F 66 66");     // ToggleTC Off
    } else if (buttonId.contains("TC CHAIN ON")) {
        interpret("77 0C C0 00 00 0A 54 6F 67 67 6C 65 54 43 20 4F 6E");        // ToggleTC On
    } else if (buttonId.contains("ALWAYS RF")) {
        interpret("77 0C C0 00 00 15 49 70 75 43 6D 64 20 74 63 67 2E 70 6C 6F 70 5F 6D 6F 64 65 20 32");  // IpuCmd tcg.plop_mode 2
    } else if (buttonId.contains("TM Mode 1")) {
        interpret("77 0C C0 00 00 0A 53 65 74 54 4D 6D 6F 64 65 20 31");  // SetTMmode 1
    }  else if (buttonId.contains("TM Mode 2")) {
        interpret("77 0C C0 00 00 0A 53 65 74 54 4D 6D 6F 64 65 20 32");  // SetTMmode 2
    }
}

void Egse::interpret(QByteArray tc) {
    if (tc.at(0) == '7') {
        transmit(QByteArray::fromHex(tc));
    } else  {
        transmitAscii(tc);
    }
}

void Egse::transmitAscii(QByteArray message) {
    message.prepend((char)(message.size() - 1));
    message = message.toHex();
    message.prepend("770cc00000");

    transmit(message);
}

void Egse::onDisconnected() {
    connected = false;
    backend->egseDisconnectedError();
}