#include "widget.h"
#include "ui_widget.h"

#include "mytcp.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    MyTcp *mytcp = new MyTcp();
    mytcp->listen(QHostAddress::Any,quint16(1111));
    ui->plainTextEdit->appendPlainText("Tcp开始监听...");
    connect(mytcp,&MyTcp::printLogin,this,&Widget::printhandle);
}



Widget::~Widget()
{
    delete ui;
}

void Widget::printhandle(QString mesg)
{
    ui->plainTextEdit->appendPlainText(mesg);
}

