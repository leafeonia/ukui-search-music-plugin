#include <QCoreApplication>
#include "networkutil.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    NetworkUtil networkUtil;
    qDebug() << "FA";
    return a.exec();
}
