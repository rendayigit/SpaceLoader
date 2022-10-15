#include "hello.h"

#include "../../common.h"

void CmdPlugin::run(QTcpSocket *sender, QByteArray /*message*/) const {
    Transmit(sender, "Hello !!");
    Log().Debug("Ran Hello !!");
}