#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include "Attractors.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("CamelCorp");
    QCoreApplication::setApplicationName("Attractors");

    Attractors w;
    w.show();
    
    return a.exec();
}
