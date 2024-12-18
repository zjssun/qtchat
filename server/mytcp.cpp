#include "mytcp.h"
#include "worker.h"
#include <QDebug>
#include <QThread>

MyTcp::MyTcp(QObject *parent) : QTcpServer(parent)
{

}
void MyTcp::incomingConnection(qintptr handle)
{
    qDebug()<<handle;
    QThread *thread = new QThread(this);
    worker *Worker = new worker();
    Worker->moveToThread(thread);
    socketThreadHash[handle] = thread;
    threadChatHash[thread] = Worker;
    connect(Worker,&worker::loginSiganl,this,&MyTcp::loginHandle);
    connect(Worker,&worker::forwardSiganl,this,&MyTcp::forwardMesgHandle);
    connect(Worker,&worker::start,Worker,&worker::mainWorker);
    connect(Worker,&worker::removeAndClose,this,&MyTcp::closeThreadHandle);
    qRegisterMetaType<qintptr>("qintptr");
    thread->start();
    emit Worker->start(handle);
}

void MyTcp::loginHandle(QString userName, qintptr handle)
{
    if(userThreadHash.contains(userName)==true){
        emit threadChatHash[socketThreadHash[handle]]->loginFailedSiganl();
    }else{
        userThreadHash[userName] = socketThreadHash[handle];
        socketThreadHash.remove(handle);
        emit threadChatHash[userThreadHash[userName]]->loginSuccessedSiganl(userName);
    }
    emit printLogin(userName+"已登录");
}

void MyTcp::forwardMesgHandle(QString targetName, QString msg)
{
    if(userThreadHash.contains(targetName)==true){
        emit threadChatHash[userThreadHash[targetName]]->sendSiganl(msg);
    }else{
        QString userName = msg.left(15).trimmed();
        emit threadChatHash[userThreadHash[userName]]->sendFailedSiganl(targetName);
    }
    QString userName = msg.left(15).trimmed();
    QString cutMesg = msg.mid(31);
    emit printLogin(userName+" 向"+targetName+"发送"+cutMesg);
}

void MyTcp::closeThreadHandle(QString userName, qintptr handle)
{
    if(userThreadHash.contains(userName)==true){
        userThreadHash[userName]->quit();
        userThreadHash[userName]->wait();
        userThreadHash.remove(userName);
    }else{
        socketThreadHash[handle]->quit();
        socketThreadHash[handle]->wait();
        socketThreadHash.remove(handle);
    }
    emit printLogin(userName+"已离开");
}


