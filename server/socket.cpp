#include "socket.h"
#include <QDebug>

socket::socket(QObject *parent, qintptr handle) : QTcpSocket(parent)
{
    this->handle = handle;
    connect(this,&socket::readyRead,this,&socket::readyReadHandle);
    connect(this,&socket::disconnected,this,&socket::disconnectedHandle);
}

void socket::readyReadHandle()
{
    QString mesg = this->readAll();
    QString userName = mesg.left(15).trimmed();
    QString targetName = mesg.mid(16,15).trimmed();
    if(targetName == "Server")
    {
        QString serverMesgType = mesg.mid(31,20).trimmed();
        if(serverMesgType == "Login")
        {
            emit loginSignal(userName,handle);
        }
    }
    else{
        emit forwardSignal(targetName,mesg);
    }
}

void socket::disconnectedHandle()
{
    emit disconnectedSiganl(userName,handle);
    qDebug()<<"disconnectedHandle";
}

void socket::loginSuccessedHandle(QString userName)
{
    QString mesg = QString("%1@%2%3").arg("Server",15).arg(userName, 15).arg("LoginSuccessful",20);
    this->write(mesg.toUtf8().data());
    this->userName = userName;
}

void socket::loginFailedHandle()
{
    QString mesg = QString("%1@%2%3").arg("Server",15).arg(userName, 15).arg("",20);
    this->write(mesg.toUtf8().data());
}

void socket::sendHandle(QString mesg)
{
    this->write(mesg.toUtf8().data());
}

void socket::sendFailedHandle(QString targetName)
{
    QString mesg = QString("%1@%2%3%4").arg("Server",15)
                                           .arg(userName, 15)
                                           .arg("TargetOffline",20)
                                           .arg(targetName);
    this->write(mesg.toUtf8().data());
}



