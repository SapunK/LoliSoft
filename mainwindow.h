#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QSqlQueryModel;
class QTableView;
class QLineEdit;
class CustomTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    void setupForm();
    void dbConnect();
    void connectWidgets();

    QWidget *m_mainWidget;

    QPushButton *m_pbSell;
    QPushButton *m_pbWarehouse;
    QPushButton *m_pbColors;
    QPushButton *m_pbModels;
    QPushButton *m_pbMaterials;
    QPushButton *m_pbShoes;
    QPushButton *m_pbEntryExitGoods;
    QPushButton *m_pbViewDocuments;
#ifdef QT_DEBUG
    QPushButton *m_pbCreateTables;
    QPushButton *m_pbFillDb;
#endif

    QSqlQueryModel *m_model;
    CustomTableView *m_table;

    QLineEdit *m_leSearch;

public slots:
    void slotSearch();

};

#endif // MAINWINDOW_H
