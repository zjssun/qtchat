#ifndef CLIENT_H
#define CLIENT_H

#include "front.h"
#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();
    Front *front;

protected:
    QTcpSocket *tcpSocket;
    bool isConnect;
    QString userName;
    QString pwd;
    QTimer timer;
    QSet<QString> targetNameSet;
    static const QHostAddress serverIP;
    static const quint16 serverPort;

    QString mesgFormat(QString payload, QString targetName = "Server", QString serverMesgType = "");
    void loginUserNameHandle();
    void closeEvent(QCloseEvent *event) override;

protected slots:
    void disconnectedHandle();
    void connectedTimeoutHandle();
    void readyReadHandle();

public slots:
    void connectServerHandle(QString userName,QString pwd);
    void exitHandle();

signals:
    void connectedFailedSignal();
    void loginSuccessedSignal();
    void loginFailedSignal();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::client *ui;
    QSqlDatabase db;
    QStringList friends;
    bool userExist(QString userName);
};

#endif // CLIENT_H
