#include "backend.h"
#include "client.h"

Backend::Backend() {
    Client::getInstance().start("127.0.0.1");
}

void Backend::sendCmd(QString text) { 
    Client::getInstance().sendCommand(text.toLocal8Bit());
}