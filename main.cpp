#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set up the OpenGL context format
    QSurfaceFormat format;
    format.setDepthBufferSize(24);  // Set the depth buffer size (optional)
    format.setVersion(2, 1);        // Set the desired OpenGL version
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;

    w.show();

    return a.exec();
}
