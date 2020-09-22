#ifndef GOODSENTRYEXIT_H
#define GOODSENTRYEXIT_H

#include <QDialog>

class QPushButton;
class QLineEdit;
class QDateEdit;
class QComboBox;
class CustomTableView;
class QSqlQueryModel;

class GoodsEntryExit : public QDialog
{
    Q_OBJECT
public:
    explicit GoodsEntryExit(QWidget *parent = nullptr);

private:

    QPushButton *m_pbNewDoc;
    QPushButton *m_pbSaveDoc;
    QPushButton *m_pbNewItem;
    QPushButton *m_pbEditItem;
    QPushButton *m_pbDeleteItem;
    QPushButton *m_pbSaveItem;

    QLineEdit *m_leDocNumber;
    QLineEdit *m_leShoe;
    QLineEdit *m_leEntryPrice;
    QLineEdit *m_leRebate;
    QLineEdit *m_leRebatePct;
    QLineEdit *m_leTax;
    QLineEdit *m_leTaxPct;
    QLineEdit *m_leDiscount;
    QLineEdit *m_leSalePrice;
    QLineEdit *m_lePriceDiff;

    QDateEdit *m_deDate;

    QComboBox *m_cbDocName;

    CustomTableView *m_table;
    QSqlQueryModel *m_model;

    QVector<QWidget *> m_vItemWidgets;

    void setupForm();
    void showHideItemWidgets(bool hide);
    void connectWidgets();

private slots:
    void setDocNumber();
};

#endif // GOODSENTRYEXIT_H
