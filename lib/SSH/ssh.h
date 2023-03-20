#ifndef SSH_H
#define SSH_H

#include <fcntl.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>

#include <string>

class SSH {
   public:
    SSH(const SSH &) = delete;
    SSH &operator=(SSH const &) = delete;
    SSH(SSH &&) = delete;
    SSH &operator=(SSH &&) = delete;
    ~SSH() = delete;

    static int fileTransfer(std::string localFile, std::string serverPath);
    static std::string runCommand(std::string command);
    static ssh_session createSession();

   private:
};

#endif  // SSH_H