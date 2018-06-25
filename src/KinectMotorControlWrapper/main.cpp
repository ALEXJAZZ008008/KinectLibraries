#include <QApplication>
#include "KinectMotorControlWrapper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("KinectMotorTest");
    a.setApplicationVersion(QObject::tr("0.0.1"));

    KinectFrontend kinect_frontend;
    kinect_frontend.show();

    return a.exec();
}
