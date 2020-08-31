#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QSqlQueryModel;
class QTableView;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    void setupForm();
    void dbConnect();

    QWidget *m_mainWidget;

    QPushButton *m_pbSell;
    QPushButton *m_pbWarehouse;
    QPushButton *m_pbColors;
    QPushButton *m_pbModels;
    QPushButton *m_pbMaterials;
    QPushButton *m_pbPopulateDb;

    QSqlQueryModel *m_model;
    QTableView *m_table;

    QLineEdit *m_leSearch;

public slots:
    void slotSearch();

};

#endif // MAINWINDOW_H
