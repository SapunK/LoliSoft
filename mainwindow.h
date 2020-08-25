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

protected:

    void setupForm();
    void dbConnect();

    QPushButton *m_pbSell;
    QPushButton *m_pbWarehouse;

    QSqlQueryModel *m_model;
    QTableView *m_table;

    QLineEdit *m_leSearch;

public slots:
    void slotMagacinClicked();
    void slotSearch();

};

#endif // MAINWINDOW_H
