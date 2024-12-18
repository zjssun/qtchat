#include "worker.h"
#include "socket.h"

worker::worker(QObject *parent) : QObject(parent)
{

}

void worker::mainWorker(qintptr handle)
{
    socket *tcpSocket = new socket(this,handle);
    //收到socket登录信号，触发worker的socketLoginHandle槽函数
    connect(tcpSocket,&socket::loginSignal,this,&worker::socketLoginHandle);
    //收到socket的转发信号，触发worker的socketForwardHandle函数
    connect(tcpSocket,&socket::forwardSignal,this,&worker::socketForwardHandle);
    //收到worker的登录成功信号，触发socket的loginSuccessedHandle函数
    connect(this,&worker::loginSuccessedSiganl,tcpSocket,&socket::loginSuccessedHandle);
    //收到worker的登录失败信号，触发socket的loginFailedHandle函数
    connect(this, &worker::loginFailedSiganl, tcpSocket, &socket::loginFailedHandle);
    //收到worker的发送消息信号，触发socket的sendHandle函数
    connect(this,&worker::sendSiganl,tcpSocket,&socket::sendHandle);
    //收到worker的发送失败信号，触发socke的sendFailedHandle函数
    connect(this,&worker::sendFailedSiganl,tcpSocket,&socket::sendFailedHandle);
    //收到socket断开连接信号，触发worker的discconnectHandle函数
    connect(tcpSocket,&socket::disconnectedSiganl,this,&worker::discconnectHandle);
    tcpSocket->setSocketDescriptor(handle);
}

void worker::socketLoginHandle(QString userName, qintptr handle)
{
    emit loginSiganl(userName,handle);
}

void worker::socketForwardHandle(QString targetName, QString mesg)
{
    emit forwardSiganl(targetName, mesg);
}

void worker::discconnectHandle(QString userName, qintptr handle)
{
    emit removeAndClose(userName, handle);
    qDebug()<<"disconnectedHandle";
}






