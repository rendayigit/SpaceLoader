#include <QtGui/QGuiApplication>
#include <QtGui/QIcon>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "backend.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    app.setOrganizationName("somename");    // TODO
    app.setOrganizationDomain("somename");  // TODO

    app.setWindowIcon(QIcon(":/images/icon.ico"));

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backend", &Backend::getInstance());
    // engine.load("qrc:/qml/splashScreen.qml"); // Bypass password screen for debugging
    engine.load("qrc:/qml/main.qml");

    return app.exec();
}

#include "main.moc"  // defined in .cpp file instead of .h file moc fails to process it properly.
