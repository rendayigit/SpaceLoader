#include "user.h"

void User::addSocket(QTcpSocket *socket) { socketInstances->append(socket); }
