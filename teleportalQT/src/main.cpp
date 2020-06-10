#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    gst_init (&argc, &argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
  //  w.show();
    //(Qt::FramelessWindowHint)
    w.setWindowFlags(Qt::FramelessWindowHint);
    //w.showFullScreen();                     //FULL SCREEN
    w.resizeWindowsManual();
    w.show();
    return a.exec();
}
