#include <gtest/gtest.h>

#include <QDirIterator>
#include <QFile>
#include <QList>
#include <QPluginLoader>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <iostream>
#include <QDebug>
#include "../../lib/Logger/logger.h"
#include "../../Commands/getUserList/getUserList.h"
#include "../Test_common.h"
#include <filesystem>
using std::filesystem::current_path;

QString getLib(QString path) {
    QString lib;
    QDirIterator iterator(path, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        QFile file(iterator.next());
        if (file.open(QIODevice::ReadOnly)) {
            if(QLibrary::isLibrary(file.fileName())) {
                if (file.fileName().contains("getuserlist", Qt::CaseInsensitive)) {
                    lib  = file.fileName().section("/",-1,-1);
                }
            }
        }
    }
    return lib;
}


TEST(Commands, getUserList) {
  

    QTcpSocket *testSocket;
    const QString path = QString::fromStdString(current_path().string()) + "/../../Libs/";
    TEST_COUT(path.toStdString());
    QString lib = path+getLib(path);
    if (lib.isNull()) {
        GTEST_FAIL() << "lib not found";
    } else {
        TEST_COUT(lib.toStdString());
        QPluginLoader loader(lib);
        if (auto *instance = loader.instance()) {
            if (auto *plugin = qobject_cast<TestPluginInterface *>(instance)) {
                plugin->run(testSocket, "getuserlist");
                qDebug()<<"plugin safely implemented";
            } else {
                GTEST_FAIL() << "qobject_cast<> returned nullptr";
            }
        }
    }

    // QString tmp = testSocket.getLastMessage();
    // std::cout << "received string " << tmp.toStdString();
    // EXPECT_EQ(tmp, "AS");
}