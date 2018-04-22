#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QApplication::addLibraryPath("./plugins");
//    QString dir = QApplication::applicationDirPath();
//    QApplication::addLibraryPath("./images");
//    w.setWindowIcon(QIcon("./images/app_icon.ico"));

    w.show();

    return a.exec();
}
