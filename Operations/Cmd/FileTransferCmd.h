#ifndef FILETRANSFERCMD_H
#define FILETRANSFERCMD_H

#include "BaseCmd.h"

class FileTransferCmd : public BaseCmd {
   public:
    FileTransferCmd() = default;

    FileTransferCmd(const FileTransferCmd &) = delete;
    FileTransferCmd &operator=(FileTransferCmd const &) = delete;
    FileTransferCmd(FileTransferCmd &&) = delete;
    FileTransferCmd &operator=(FileTransferCmd &&) = delete;
    ~FileTransferCmd() = default;

    QString getDestinationDir() const { return DestinationDir; }
    void setDestinationDir(const QString &destinationDir) { DestinationDir = destinationDir; }

   private:
    QString DestinationDir;
};

#endif  // FILETRANSFERCMD_H
