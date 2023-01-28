#ifndef SSH_H
#define SSH_H

#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QtCore/QThread>
#include <fstream>

#include "../Logger/logger.h"

class SSH {
    public:
        SSH(const SSH &) = delete;
        SSH &operator=(SSH const &) = delete;
        SSH(SSH &&) = delete;
        SSH &operator=(SSH &&) = delete;

        static int fileTransfer(QString localFile, QString serverPath);
        static QString runCommand(QString command);
        static int downloadFile(QString serverPath, QString localPath);

    private:
        static int initConnection();
};

#endif // SSH_H