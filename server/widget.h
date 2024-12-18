#ifndef WIDGET_H
#define WIDGET_H

#include "worker.h"
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void printhandle(QString mesg);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
