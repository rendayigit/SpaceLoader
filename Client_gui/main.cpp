#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtGui/QIcon>

#include "client.h"

class Backend : public QObject {
Q_OBJECT
    QString textFieldContent;
    QTimer *timer = new QTimer(this);
    qint64 initialEpochMs;
    qint64 relativeEpochMs;

public:
    Backend() {
        initialEpochMs = QDateTime::currentMSecsSinceEpoch();

        connect(timer, SIGNAL(timeout()), this, SLOT(update()));

        timer->setTimerType(Qt::TimerType::PreciseTimer);
        timer->start(1);
    };

public slots:

    void update() {
        relativeEpochMs = QDateTime::currentMSecsSinceEpoch() - initialEpochMs;
//        qDebug() << relativeEpochMs << Qt::endl;

        emit printTime(relativeEpochMs);

//        if(relativeEpochMs > 100)
//            timer->stop();
    }

    Q_INVOKABLE void welcomeText(QString name) {
//        qDebug() << "welcomeText called from qml. param is " << name;
        emit setName("Welcome, " + name);
    }

    Q_INVOKABLE void openFile(QString filePath) {
        const QUrl url(filePath);
        QFile file(url.toLocalFile());

        QString lines = "";
        if (file.open(QIODevice::ReadOnly)) {
            while (!file.atEnd()) {
                QString line(file.readLine());
                lines += line + '\n';
            }
        }
        file.close();

        emit readText(lines);
    }

    Q_INVOKABLE void showHideRectangle(bool isChecked) {
        emit isVisible(isChecked);
    }

    Q_INVOKABLE QDateTime getCurrentDateTime() const {
        return QDateTime::currentDateTime();
    }

    Q_INVOKABLE void setTextField(QString content) {
        textFieldContent = content;
    }

    Q_INVOKABLE void writeFile(QString filePath) const {
        const QUrl url(filePath);
        QFile file(url.toLocalFile());

        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(textFieldContent.toUtf8());
            file.close();

        } else {
            qDebug() << "save error";
        }
    }

signals:

    void setName(QString);

    void readText(QString);

    void printTime(qint64);

    void isVisible(bool);
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    app.setOrganizationName("somename");      //TODO
    app.setOrganizationDomain("somename");   //TODO

    app.setWindowIcon(QIcon(":/images/icon.ico"));

    QQmlApplicationEngine engine;
    Backend data;

    engine.rootContext()->setContextProperty("backend", &data);
    engine.load("qrc:/qml/splashScreen.qml"); // Bypass password screen for debugging
//    engine.load("qrc:/qml/main.qml");

    // QList<QString> commandArguments;
    // commandArguments.append("127.0.0.1");
    // Client::getInstance().start(commandArguments);

    return app.exec();
}

#include "main.moc" // defined in .cpp file instead of .h file moc fails to process it properly.
