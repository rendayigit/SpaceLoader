#include <QtCore/QtCore>

class Backend : public QObject {
    Q_OBJECT

   public:
   Backend(); // TODO move to private
    static auto &getInstance() {
        static Backend instance ;
        return instance;
    }

   public slots:
    Q_INVOKABLE void sendCmd(QString text);

   signals:
    void getReceivedText(QString text);

   private:
    
};