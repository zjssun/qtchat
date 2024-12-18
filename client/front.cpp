#include "front.h"
#include "ui_front.h"

#include <QMessageBox>
#include <QDebug>
Front::Front(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Front)
{
    ui->setupUi(this);
    ui->reg_w->hide();
    ui->regBtn->hide();
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

Front::~Front()
{
    delete ui;
}

void Front::connectedFaileHandle()
{
    QMessageBox::warning(this,"Warning","连接超时！",QMessageBox::Ok);
}

void Front::loginSuccessedHandle()
{
    hide();
//    ui->loginButton->setDisabled(false);
}

void Front::loginFaileHandle()
{
    QMessageBox::warning(this,"Warning","登录失败！",QMessageBox::Ok);
}

void Front::on_loginBtn_clicked()
{
    QSqlQuery query(db);
    QString userName = ui->userEdit->text();
    QString pwd = ui->pwdEdit->text();
    if(userName!="" && pwd!=""){
        if(userExist(userName)){
            query.prepare("select pwd from users where username = :userName");
            query.bindValue(":userName",userName);
            if (!query.exec()) {
                qDebug() << "Query execution failed:";
                return;
            }
            if(query.next()){
                QString passworld = query.value(0).toString();
                qDebug() << passworld;
                if(pwd == passworld){
                    emit UserNameSignal(userName,pwd);
                }else{
                    QMessageBox::warning(this,"Warning","密码错误！",QMessageBox::Ok);
                }
            }
        }else{
            QMessageBox::warning(this,"Warning","此用户未注册！",QMessageBox::Ok);
        }
    }else{
        QMessageBox::warning(this,"Warning","请填写完整！",QMessageBox::Ok);
    }
}

void Front::on_exitBtn_clicked()
{
    emit exitSignal();
    this->close();
}

void Front::on_switchBtn_clicked()
{
    if(ui->regBtn->isHidden()){
        ui->loginBtn->hide();
        ui->login_w->hide();
        ui->regBtn->show();
        ui->reg_w->show();
        ui->switchBtn->setText("已有账户？");
    }else{
        ui->loginBtn->show();
        ui->login_w->show();
        ui->regBtn->hide();
        ui->reg_w->hide();
        ui->switchBtn->setText("还未注册？");
    }
}

bool Front::userExist(QString userName)
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

void Front::on_regBtn_clicked()
{
    QString userName = ui->lineEdit->text();
    QString pwd = ui->lineEdit_2->text();
    QString rpwd = ui->lineEdit_3->text();
    if(userName !="" && pwd !="" && rpwd !=""){
        if(userExist(userName)){
            QMessageBox::warning(this,"Warning","用户已存在！",QMessageBox::Ok);
        }else{
            if(pwd == rpwd){
                QSqlQuery query(db);
                query.prepare("INSERT INTO users (username, pwd, friend) VALUES (:username, :pwd, :friend)");
                query.bindValue(":username",userName);
                query.bindValue(":pwd",pwd);
                query.bindValue(":friend ","[]");
                if (!query.exec()) {
                    qDebug() << "Query execution failed:";
                    return;
                }else {
                    qDebug() << "Data inserted successfully!";
                }
                emit UserNameSignal(userName,pwd);
            }else{
                QMessageBox::warning(this,"Warning","确认密码不一致！",QMessageBox::Ok);
            }
        }
    }else{
        QMessageBox::warning(this,"Warning","请填写完整！",QMessageBox::Ok);
    }
}

