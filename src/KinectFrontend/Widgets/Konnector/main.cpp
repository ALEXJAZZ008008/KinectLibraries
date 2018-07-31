#include <QApplication>
#include "konnector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Konnector");
    a.setApplicationVersion(QObject::tr("0.0.1"));

    Konnector konnector;
    konnector.konnector_main();

    return a.exec();
}
