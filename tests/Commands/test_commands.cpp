#include <gtest/gtest.h>

#include <QPluginLoader>

#include "../../Commands/getUserList/getUserList.h"
#include "../../lib/Logger/logger.h"
#include "../Test_common.h"
#include "../common.h"

QString getLib(QString lib) {
    QString path = Paths().getCommandsDir();
    QDirIterator iterator(path, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        QFile file(iterator.next());
        if (file.open(QIODevice::ReadOnly)) {
            if (QLibrary::isLibrary(file.fileName())) {
                if (file.fileName().contains(lib, Qt::CaseInsensitive)) {
                    return file.fileName().section("/", -1, -1);
                }
            }
        }
    }
    return "";
}

TEST(Commands, getUserList) {
    QTcpSocket *testSocket{};
    const QString lib = getLib("getuserlist");

    if (lib.isNull()) {
        GTEST_FAIL() << "lib not found";
    }

    QPluginLoader loader(lib);
    if (auto *instance = loader.instance()) {
        if (auto *plugin = qobject_cast<TestPluginInterface *>(instance)) {
            plugin->run(testSocket, "getuserlist");
        } else {
            GTEST_FAIL() << "qobject_cast<> returned nullptr";
        }
    }

    // TODO
    // QString tmp = testSocket.getLastMessage();
    // std::cout << "received string " << tmp.toStdString();
    // EXPECT_EQ(tmp, "AS");
}