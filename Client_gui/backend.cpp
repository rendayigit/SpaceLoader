#include "backend.h"

#include "client.h"


Backend::Backend() { Client::getInstance().start("127.0.0.1"); }

void Backend::getTerminalData(QString text) {
    Client::getInstance().sendCommand(
        text.mid(text.lastIndexOf(">") + 2, text.size()).toLocal8Bit());
}