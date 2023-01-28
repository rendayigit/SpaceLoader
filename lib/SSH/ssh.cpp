#include "ssh.h"

ssh_session session;

int SSH::fileTransfer(QString localFile, QString serverPath) {
    int rc = initConnection();

    if(rc == -1) {
        return -1;
    }

    std::ifstream file(localFile.toStdString(), std::ios::binary);
    if (!file.is_open()) {
        QString errorMessage = "Error opening file";
        Log().Error(errorMessage);
        return -1;
    }

    file.seekg(0, file.end);
    size_t fileSize = file.tellg();
    file.seekg(0, file.beg);

    char* buffer = new char[fileSize];
    file.read(buffer, fileSize);

    sftp_session sftp = sftp_new(session);
    if (sftp == nullptr) {
        QString errorMessage =
            "Error allocating SFTP session: " + QString::fromStdString(ssh_get_error(session));
        Log().Error(errorMessage);
        return -1;
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK) {
        QString errorMessage =
            "Error initializing SFTP session: " + QString::fromStdString(ssh_get_error(session));
        Log().Error(errorMessage);
        return -1;
    }

    QString fileName = localFile.split("/").last();
    QString remoteFilePath = serverPath + "/" + fileName;

    sftp_file fileHandle =
        sftp_open(sftp, remoteFilePath.toLocal8Bit(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fileHandle == nullptr) {
        QString errorMessage = "Error opening file on remote server: " +
                               QString::fromStdString(ssh_get_error(session));
        Log().Error(errorMessage);
        return -1;
    }

    rc = sftp_write(fileHandle, buffer, fileSize);
    if (rc < 0) {
        QString errorMessage = "Error writing file to remote server: " +
                               QString::fromStdString(ssh_get_error(session));
        Log().Error(errorMessage);
        return -1;
    }

    Log().Info("File transfer complete");

    sftp_close(fileHandle);
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}

QString SSH::runCommand(QString command) {
    ssh_channel channel;
    int rc;

    channel = ssh_channel_new(session);
    if (channel == NULL) return "";

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        return "";
    }

    rc = ssh_channel_request_exec(channel, "ls -l");
    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return "";
    }

    char buffer[256];
    int nbytes;

    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        if (fwrite(buffer, 1, nbytes, stdout) != nbytes) {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
            return "";
        }
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    if (nbytes < 0) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return "";
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return QString::fromUtf8(buffer, nbytes);
}

int SSH::downloadFile(QString serverPath, QString localPath) {
    // TODO this function might be implement with run command on server.
}

int SSH::initConnection() {
    session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.2");
    ssh_options_set(session, SSH_OPTIONS_USER, "Administrator");

    int rc = ssh_connect(session);

    if (rc != SSH_OK) {
        QString errorMessage =
            "Error connecting to host: " + QString::fromStdString(ssh_get_error(session));
        Log().Error(errorMessage);
        return -1;
    }

    rc = ssh_userauth_password(session, "Administrator", "uyssw");
    if (rc != SSH_OK) {
        QString errorMessage =
            "Error authenticating with password: " + QString::fromStdString(ssh_get_error(session));
        Log().Error(errorMessage);
        return -1;
    }

    return rc;
}