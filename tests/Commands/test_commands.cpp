#include <gtest/gtest.h>

#include <QtCore/QPluginLoader>

#include "../../Commands/hello/hello.h"
#include "../../lib/Logger/logger.h"
#include "../Test_common.h"
#include "../../path.h"

QString getLib(QString lib) {
    QString path = Paths().getCmdsDir();
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

TEST(Commands, hello) {
    QTcpSocket *testSocket{};
    const QString lib = getLib("hello");

    if (lib.isNull()) {
        GTEST_FAIL() << "lib not found";
    }

    QPluginLoader loader(lib);
    if (auto *instance = loader.instance()) {
        if (auto *plugin = qobject_cast<CmdPluginInterface *>(instance)) {
            plugin->run(testSocket, "hello");
        } else {
            GTEST_FAIL() << "qobject_cast<> returned nullptr";
        }
    }

    // TODO
    // QString tmp = testSocket.getLastMessage();
    // std::cout << "received string " << tmp.toStdString();
    // EXPECT_EQ(tmp, "hello");
}