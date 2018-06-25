#include <QApplication>
#include "KinectFrontend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("KinectFrontend");
    a.setApplicationVersion(QObject::tr("0.0.1"));

    KinectFrontend::getInstance().show();

    return a.exec();
}
