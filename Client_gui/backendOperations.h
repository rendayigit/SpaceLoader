#ifndef BACKENDOPERATIONS_H
#define BACKENDOPERATIONS_H

#include <QtCore/QFile>
#include <QtCore/QThread>

#include "../constants.h"
#include "../lib/Logger/logger.h"
#include "backend.h"
#include "iostream"

class BackendOperations {
   public:
     BackendOperations(Backend *mahmut);
        
    void fileTransfer(QString localFile, QString serverPath);
    void listen(QString ipPort);
    void parse(QString text);
    void start(QString ip);

    private:
        Backend* backend;

};

#endif  // BACKENDOPERATIONS_H
