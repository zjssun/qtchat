#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class worker : public QObject
{
    Q_OBJECT
public:
    explicit worker(QObject *parent = nullptr);
    void run(qintptr handle);
    void mainWorker(qintptr handle);
public slots:
    void socketLoginHandle(QString userName, qintptr handle);
    void socketForwardHandle(QString targetName,QString mesg);
    void discconnectHandle(QString userName, qintptr handle);

signals:
    void loginSiganl(QString userName, qintptr handle);
    void forwardSiganl(QString targetName, QString mesg);
    void sendSiganl(QString mesg);
    void removeAndClose(QString userName, qintptr handle);

    void loginSuccessedSiganl(QString userName);
    void loginFailedSiganl();
    void sendFailedSiganl(QString targetName);

    void start(qintptr handle);
};

#endif // WORKER_H
