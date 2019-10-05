#include "widget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    Widget w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();


    return a.exec();
}
