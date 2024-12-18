#include "client.h"
#include "front.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client w;

    Front *f = new Front();
    f->setWindowFlags(Qt::WindowCloseButtonHint);
    w.front = f;

    QObject::connect(f,&Front::UserNameSignal,&w,&client::connectServerHandle);
    QObject::connect(f,&Front::exitSignal,&w,&client::exitHandle);
    QObject::connect(&w,&client::loginSuccessedSignal,f,&Front::loginSuccessedHandle);
    QObject::connect(&w,&client::loginFailedSignal,f,&Front::loginFaileHandle);
    QObject::connect(&w,&client::connectedFailedSignal,f,&Front::connectedFaileHandle);
    f->show();

    return a.exec();
}
