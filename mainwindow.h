#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QLineEdit>
#include <QGroupBox>

#include "appconsts.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    void setupForm();

    QPushButton *m_pbProdazba;
    QPushButton *m_pbMagacin;

    QLineEdit *m_leSearch;

};

#endif // MAINWINDOW_H
