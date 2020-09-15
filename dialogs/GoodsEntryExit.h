#ifndef GOODSENTRYEXIT_H
#define GOODSENTRYEXIT_H

#include <QDialog>

class QPushButton;
class QLineEdit;
class QDateEdit;

class GoodsEntryExit : public QDialog
{
    Q_OBJECT
public:
    explicit GoodsEntryExit(QWidget *parent = nullptr);

private:

    QPushButton *m_pbNewDoc;
    QPushButton *m_pbInsertDoc;
    QPushButton *m_pbNewItem;
    QPushButton *m_pbEditItem;
    QPushButton *m_pbDeleteItem;

    QLineEdit *m_leDocName;
    QLineEdit *m_leDocNumber;
    QLineEdit *m_leEntryPrice;
    QLineEdit *m_leRebate;
    QLineEdit *m_leRebatePct;
    QLineEdit *m_leTax;
    QLineEdit *m_leTaxPct;
    QLineEdit *m_leDiscount;
    QLineEdit *m_leSalePrice;
    QLineEdit *m_lePriceDiff;

    QDateEdit *m_deDate;

    void setupForm();

};

#endif // GOODSENTRYEXIT_H
