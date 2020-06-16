#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    gst_init (&argc, &argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    w.setWindowFlags(Qt::FramelessWindowHint);      //ENABLE FULLSCREEN WINDOW FOR APP TO RUN
    w.resizeWindowsManual();
    w.show();
    return a.exec();
}
