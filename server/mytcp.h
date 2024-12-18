#ifndef MYTCP_H
#define MYTCP_H

#include "worker.h"
#include <QObject>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>

class MyTcp : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcp(QObject *parent = 0);
    void incomingConnection(qintptr handle) override;

signals:
    //登录成功消息
    void printLogin(QString userName);

protected slots:
    //登录成功处理
    void loginHandle(QString userName,qintptr handle);
    //转发消息
    void forwardMesgHandle(QString targetName, QString msg);
    //断开处理
    void closeThreadHandle(QString userName, qintptr handle);

protected:
    QHash<qintptr,QThread*> socketThreadHash;

    QHash<QString,QThread*> userThreadHash;

    QHash<QThread*,worker*> threadChatHash;
};
#endif // MYTCP_H
