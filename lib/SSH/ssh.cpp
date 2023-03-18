#include "ssh.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

int SSH::fileTransfer(std::string localFile, std::string serverPath) {
    ssh_session session = createSession();

    if (session == nullptr) {
        std::cout << "Session is null";
        return -1;
    }

    std::ifstream file(localFile, std::ios::binary);
    if (not file.is_open()) {
        std::cout << "Error opening file";
        return -1;
    }

    file.seekg(0, std::ifstream::end);
    ssize_t fileSize = file.tellg();
    file.seekg(0, std::ifstream::beg);

    auto buffer = std::make_unique<char[]>(fileSize);
    file.read(buffer.get(), fileSize);

    sftp_session sftp = sftp_new(session);
    if (sftp == nullptr) {
        std::cout << "Error allocating SFTP session: " << ssh_get_error(session);
        return -1;
    }

    if (sftp_init(sftp) != SSH_OK) {
        std::cout << "Error initializing SFTP session: " << ssh_get_error(session);
        return -1;
    }

    std::string fileName = localFile.substr(localFile.find_last_of('/') + 1);
    std::string remoteFilePath = serverPath + "/" + fileName;

    sftp_file fileHandle = sftp_open(sftp, remoteFilePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

    if (fileHandle == nullptr) {
        std::cout << "Error opening file on remote server: " << ssh_get_error(session);
        return -1;
    }

    if (sftp_write(fileHandle, buffer.get(), fileSize) < 0) {
        std::cout << "Error writing file to remote server: " << ssh_get_error(session);
        return -1;
    }

    std::cout << "File transfer complete";

    sftp_close(fileHandle);
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}

// QString SSH::runCommand(ssh_session session, QString command) {
//     ssh_channel channel;
//     int returnCode;

//     if (session == nullptr) {
//         QString errorMessage = "Session is null";
//         Log().Error(errorMessage);
//         qCritical() << errorMessage;
//         return;
//     }

//     channel = ssh_channel_new(session);
//     if (channel == NULL) return;

//     returnCode = ssh_channel_open_session(channel);
//     if (returnCode != SSH_OK) {
//         ssh_channel_free(channel);
//         return;
//     }

//     returnCode = ssh_channel_request_exec(channel, command.toStdString().c_str());
//     if (returnCode != SSH_OK) {
//         ssh_channel_close(channel);
//         ssh_channel_free(channel);
//         return;
//     }

//     char buffer[256];
//     int nbytes;

//     nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
//     while (nbytes > 0) {
//         if (fwrite(buffer, 1, nbytes, stdout) != nbytes) {
//             ssh_channel_close(channel);
//             ssh_channel_free(channel);
//             return;
//         }
//         nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
//     }

//     ssh_channel_close(channel);
//     ssh_channel_free(channel);

//     if (nbytes < 0) {
//         return;
//     }

//     ssh_channel_send_eof(channel);

//     return QString::fromUtf8(buffer, nbytes);
// }

// int SSH::downloadFile(ssh_session session, QString serverPath, QString localPath) {
// TODO this function might be implement with run command on server.
// }

ssh_session SSH::createSession() {
    ssh_session session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.169.128");
    ssh_options_set(session, SSH_OPTIONS_USER, "pop");

    if (ssh_connect(session) != SSH_OK) {
        std::cout << "Error connecting to host: " << ssh_get_error(session);
        return nullptr;
    }

    if (ssh_userauth_password(session, "pop", "1") != SSH_OK) {
        std::cout << "Error authenticating with password: " << ssh_get_error(session);
        return nullptr;
    }

    return session;
}