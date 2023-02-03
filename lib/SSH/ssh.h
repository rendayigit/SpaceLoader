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

        static int fileTransfer(ssh_session session, QString localFile, QString serverPath);
        static QString runCommand(ssh_session session, QString command);
        static int downloadFile(ssh_session session, QString serverPath, QString localPath);
        static ssh_session createSession();

    private:
};

#endif // SSH_H