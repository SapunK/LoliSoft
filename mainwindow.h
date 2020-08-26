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
    QPushButton *m_pbColors;
    QPushButton *m_pbModels;
    QPushButton *m_pbMaterials;

    QSqlQueryModel *m_model;
    QTableView *m_table;

    QLineEdit *m_leSearch;

public slots:
    void slotMagacinClicked();
    void slotSearch();
    void setPbProperties(QPushButton &pb);

};

#endif // MAINWINDOW_H
