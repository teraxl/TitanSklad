#include "widget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    QTabWidget tab;

    Widget w;
    Widget w2;
    tab.addTab(&w, "#1");
    tab.addTab(&w2, "#2");
    tab.setWindowState(Qt::WindowMaximized);
    tab.show();


    return a.exec();
}
