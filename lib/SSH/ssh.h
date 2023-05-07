#ifndef SSH_H
#define SSH_H

#include <fcntl.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>

#include <string>

int sessionIsNull = 0;
int channelIsNull = 1;
int sessionCanNotOpenChannel = 2;
int failExecRequest = 3;
int errorWhileWriting = 4;
int emptyNBytes = 5;
int succesfullRunCommand = 6;
std::string outputBuffer = "";

class SSH {
   public:
    SSH(const SSH &) = delete;
    SSH &operator=(SSH const &) = delete;
    SSH(SSH &&) = delete;
    SSH &operator=(SSH &&) = delete;
    ~SSH() = delete;

    static int fileTransfer(std::string localFile, std::string serverPath);
    static int runCommand(std::string command);
    static ssh_session createSession();
    static std::string getOutputBuffer();
};

#endif  // SSH_H