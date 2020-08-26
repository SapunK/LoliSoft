#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //TODO fix the stylesheet
    MainWindow w;
    w.setStyleSheet("styles.css");
    w.show();

    return a.exec();
}
