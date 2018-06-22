#include <QCoreApplication>

#include <iostream>
#include "kinectdepthimageoutputwrapper.h"

using namespace std;
using namespace KinectDepthImageOutputWrapper;

int start()
{
    cout << "KinectDepthImageOutputWrapper main:    Start" << endl;

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    start();

    KinectDepthImageOutputToConsole();

    return a.exec();
}
