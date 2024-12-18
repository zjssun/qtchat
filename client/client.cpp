#include "client.h"
#include "ui_client.h"

#include <QHostAddress>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

const QHostAddress client::serverIP = QHostAddress("127.0.0.1");
const quint16 client::serverPort = 1111;

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    tcpSocket = nullptr;
    isConnect = false;
    userName = nullptr;
    pwd = nullptr;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setDatabaseName("qtchat");
  //db.setPassword("123456");
    if(!db.open()){
        qDebug()<<"Database connection failed!";
    }else{
      qDebug()<<"Database connection Success!\n";
    }
}

client::~client()
{
    delete ui;
}

void client::connectServerHandle(QString userName,QString pwd)
{
    this->userName = userName;
    this->pwd = pwd;
    qDebug()<<"connectServerHandle";
    QSqlQuery query(db);
    query.prepare("SELECT friend FROM users WHERE userName = :userName");
    query.bindValue(":userName", userName);
    if (!query.exec()) {
        qDebug() << "Query execution failed";
        return;
    }
    if (query.next()) {
        QString friendData = query.value(0).toString();
        if(friendData!=""){
            friends = friendData.split(',');
            ui->comboBox->addItems(friends);
        }
    } else {
        qDebug() << "No user found with userName =" << userName;
    }
    if(isConnect != true){
        if(tcpSocket == nullptr){
            tcpSocket = new QTcpSocket(this);
        }
        connect(tcpSocket,&QTcpSocket::connected,this,&client::loginUserNameHandle);
        qDebug()<<"connectedsignal";
        connect(&timer,&QTimer::timeout,this,&client::connectedTimeoutHandle);
        qDebug()<<"timeoutsignal";
        connect(tcpSocket,&QTcpSocket::readyRead,this,&client::readyReadHandle);
        qDebug()<<"readyReadsignal";
        tcpSocket->connectToHost(serverIP,serverPort);
        if(timer.isActive() != true){
            timer.start(10000);
        }
    }else{
        loginUserNameHandle();
    }
}

QString client::mesgFormat(QString payload, QString targetName, QString serverMesgType)
{
    QString mesg;
    if(targetName == "Server" && serverMesgType !=""){
        mesg = QString("%1@%2%3").arg(userName,15)
                .arg("Server",15).arg(serverMesgType,20);
    }else if(targetName != "Server" && serverMesgType==""){
        mesg = QString("%1@%2%3").arg(userName, 15)
                .arg(targetName, 15)
                .arg(payload);
    }
    return  mesg;
}

void client::closeEvent(QCloseEvent *event)
{
    disconnect(tcpSocket,&QTcpSocket::disconnected,this,&client::disconnectedHandle);
    timer.stop();
    isConnect = false;
    if(tcpSocket != nullptr){
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
    event->accept();
}

void client::disconnectedHandle()
{
    isConnect = false;
    QMessageBox::warning(this,"Warning","Disconnect from the server",QMessageBox::Ok);
    front->show();
    hide();
}

void client::connectedTimeoutHandle()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    timer.stop();
    emit connectedFailedSignal();
}

void client::readyReadHandle()
{
    QString mesg = tcpSocket->readAll();
    qDebug()<<"mesg="<<mesg;
    QString targetName = mesg.left(15).trimmed();
    QString payload = mesg.mid(31);
    qDebug()<<"payload="<<payload;
    if(targetName == "Server"){
        QString serverMesgType = payload.left(20).trimmed();
        qDebug()<<"serverMesgType="<<serverMesgType;
        if(serverMesgType == "LoginSuccessful"){
            ui->userName->setText(userName);
            show();
            emit loginSuccessedSignal();
        }
        else if(serverMesgType == "LoginFailed"){
            emit loginFailedSignal();
        }
        else if(serverMesgType == "TargetOffline"){
            QString offlineTarget = payload.mid(20);
            QString showMesg = QString("[Server]: \"%1\" is offline!").arg(offlineTarget);
            ui->Browser->append(showMesg);
        }
    }
    else{
        payload = payload.trimmed();
        QString showMesg = QString("[%1]:%2").arg(targetName,payload);
        ui->Browser->append(showMesg);
    }
}

void client::loginUserNameHandle()
{
    timer.stop();
    isConnect = true;
    connect(tcpSocket,&QTcpSocket::disconnected,this,&client::disconnectedHandle);
    QString mesg = mesgFormat("","Server","Login");
    qDebug()<<mesg;
    tcpSocket->write(mesg.toUtf8().data());
}

void client::exitHandle()
{
    timer.stop();
    isConnect = false;
    if(tcpSocket != nullptr)
    {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
    this->close();
}

void client::on_pushButton_clicked()
{
    QString targetName = ui->comboBox->currentText();
    if(targetName!=""){
        if(targetName.compare("Server")==0)
        {
            return;
        }
        if(targetNameSet.contains(targetName) != true)
        {
            targetNameSet.insert(targetName);
        }
        QString payload = ui->lineEdit->text();
        QString mesg = mesgFormat(payload,targetName);
        tcpSocket->write(mesg.toUtf8().data());
        ui->Browser->append(QString("to [%1]: %2").arg(targetName, payload));
        ui->lineEdit->clear();
    }else{
        QMessageBox::warning(this,"Warning","请选择好友",QMessageBox::Ok);
    }
}


void client::on_pushButton_2_clicked()
{
    QString targetName = ui->comboBox->currentText();
    if(targetName != ""){
        if(!friends.contains(targetName) && userExist(targetName)){
            QSqlQuery query(db);
            QString newfrineds = friends.join(',');
            QString newfrineds_s = "";
            if(newfrineds==""){
                newfrineds_s = targetName;
            }else {
                newfrineds_s = newfrineds+","+targetName;
            }
            query.prepare("update users set friend = :friends where username = :userName");
            query.bindValue(":friends",newfrineds_s);
            query.bindValue(":userName",userName);
            if (!query.exec()) {
                qDebug() << "Query execution failed";
                return;
            }
            if(userExist(targetName)){
                query.prepare("SELECT friend FROM users WHERE username = :userName");
                query.bindValue(":userName", targetName);
                if (!query.exec()) {
                    qDebug() << "Query execution failed";
                    return;
                }
                if (query.next()) {
                    QString friendData = query.value(0).toString();
                    if(friendData==""){
                        friendData = userName;
                    }else{
                        friendData = friendData + "," + userName;
                    }
                    qDebug() << friendData;
                    query.prepare("update users set friend = :friendData where username=:targetName");
                    query.bindValue(":friendData",friendData);
                    query.bindValue(":targetName",targetName);
                    if (!query.exec()) {
                        qDebug() << "Query execution failed";
                        return;
                    }
                }
        }else{
            QMessageBox::warning(this,"Warning","没有该用户！",QMessageBox::Ok);
        }
        QMessageBox::warning(this,"Succeed","添加成功！",QMessageBox::Ok);
    }else{
            QMessageBox::warning(this,"Warning","已有该好友或没有该用户!",QMessageBox::Ok);
        }
   }else{
        QMessageBox::warning(this,"Warning","没内容!",QMessageBox::Ok);
    }
}

bool client::userExist(QString userName)
{
    QSqlQuery query(db);
    bool result = false;
    query.prepare("select count(*) from users where username = :userName");
    query.bindValue(":userName",userName);
    if (!query.exec()) {
        qDebug() << "Query execution failed:";
        return -1;
    }
    if (query.next()) {
        int count = query.value(0).toInt();
        if(count>0){
            result = true;
        }else{
            result = false;
        }
    }
    return result;
}

