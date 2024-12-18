#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>

class socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit socket(QObject *parent = nullptr,qintptr handle = NULL);

protected:
    qintptr handle;
    QString userName;

protected slots:
    void readyReadHandle();
    void disconnectedHandle();

public slots:
    void loginSuccessedHandle(QString userName);
    void loginFailedHandle();
    void sendHandle(QString mesg);
    void sendFailedHandle(QString targetName);

signals:
    void loginSignal(QString userName, qintptr handle);
    void forwardSignal(QString targetName, QString mesg);
    void disconnectedSiganl(QString userName, qintptr handle);
};

#endif // SOCKET_H
