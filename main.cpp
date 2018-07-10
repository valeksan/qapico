#include "mainwindow.h"
#include <QApplication>
#include <QNetworkProxy>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::NoProxy);
//    proxy.setType(QNetworkProxy::HttpProxy);
//    proxy.setHostName("127.0.0.1");
//    proxy.setPort(3128);
    QNetworkProxy::setApplicationProxy(proxy);

    return a.exec();
}
