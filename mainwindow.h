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
#include <QTabWidget>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QSqlQueryModel>
#include <QTableView>
#include <QShortcut>

#include "appconsts.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void setupForm();
    void dbConnect();

    QPushButton *m_pbProdazba;
    QPushButton *m_pbMagacin;

    QSqlQueryModel *m_model;
    QTableView *m_table;

    QLineEdit *m_leSearch;

public slots:
    void slotMagacinClicked();
    void slotSearchLE();

};

#endif // MAINWINDOW_H
