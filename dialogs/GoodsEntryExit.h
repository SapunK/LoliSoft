#ifndef GOODSENTRYEXIT_H
#define GOODSENTRYEXIT_H

#include <QDialog>

class QPushButton;
class QLineEdit;
class QDateEdit;
class QComboBox;
class CustomTableView;
class QSqlQueryModel;
class CustomDoubleLE;
class CustomIntLE;

namespace GoodsEntryExit_NS {
enum EEditItem{
    shoe,
    size,
    quantity,
    entry_price,
    rebate,
    rebate_pct,
    price_after_rebate,
    tax,
    tax_pct,
    margin,
    marginPct,
    discount,
    discoun_pct,
    price_diff,
    sale_price,
    sum_value
};
}

class GoodsEntryExit : public QDialog
{
    Q_OBJECT
public:
    explicit GoodsEntryExit(QWidget *parent = nullptr);

private:
    
    int m_docId;
    int m_itemId;

    QPushButton *m_pbNewDoc;
    QPushButton *m_pbSaveDoc;
    QPushButton *m_pbNewItem;
    QPushButton *m_pbEditItem;
    QPushButton *m_pbDeleteItem;
    QPushButton *m_pbSaveItem;

    QLineEdit *m_leDocNumber;

    CustomIntLE *m_leSize;
    CustomIntLE *m_leQuantity;

    CustomDoubleLE *m_leEntryPrice;
    CustomDoubleLE *m_leRebate;
    CustomDoubleLE *m_leTax;
    CustomDoubleLE *m_leMargin;
    CustomDoubleLE *m_leDiscountPct;
    CustomDoubleLE *m_leDiscount;
    CustomDoubleLE *m_leSalePrice;
    CustomDoubleLE *m_lePriceDiff;
    CustomDoubleLE *m_leRebatePct;
    CustomDoubleLE *m_lePrcAfterRebate;
    CustomDoubleLE *m_leTaxPct;
    CustomDoubleLE *m_leMarginPct;
    CustomDoubleLE *m_leDocValue;

    QDateEdit *m_deDate;

    QComboBox *m_cbShoe;
    QComboBox *m_cbDocType;

    CustomTableView *m_table;
    QSqlQueryModel *m_model;

    QVector<QWidget *> m_vItemWidgets;

    void setupForm();
    void showHideItemWidgets(bool hide);
    void connectWidgets();
    bool validateItem();
    void clearItemFields();
    void updateFields();

    bool event(QEvent *event);
private slots:
    void setDocNumber();
    void insertUpdateItem();
    void editItem();
    void deleteItem();
    void saveDoc();
};


#endif // GOODSENTRYEXIT_H
