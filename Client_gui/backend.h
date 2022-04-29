#include <QtCore/QtCore>

class Backend : public QObject {
Q_OBJECT

public:
    Backend();

public slots:

    Q_INVOKABLE void sendCmd(QString text);

signals:
    QString getReceivedText();
};