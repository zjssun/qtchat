#ifndef FRONT_H
#define FRONT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>

namespace Ui {
class Front;
}

class Front : public QWidget
{
    Q_OBJECT

public:
    explicit Front(QWidget *parent = 0);
    ~Front();

public slots:
    void connectedFaileHandle();
    void loginSuccessedHandle();
    void loginFaileHandle();

signals:
    void UserNameSignal(QString userName,QString pwd);
    void exitSignal();


private slots:

    void on_loginBtn_clicked();

    void on_exitBtn_clicked();

    void on_switchBtn_clicked();

    void on_regBtn_clicked();

private:
    Ui::Front *ui;
    QSqlDatabase db;
    bool userExist(QString userName);
};

#endif // FRONT_H
