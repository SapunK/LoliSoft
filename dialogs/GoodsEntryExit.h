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

enum EEditItem{
    shoe,
    size,
    quantity,
    entryPrice,
    rebate,
    rebatePct,
    priceAfterRebate,
    tax,
    taxPct,
    margin,
    marginPct,
    discount,
    discountPct,
    priceDiff,
    salePrice
};

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

    QLineEdit *m_leShoe;
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
    CustomDoubleLE *m_leSumValue;

    QDateEdit *m_deDate;

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

private slots:
    void setDocNumber();
    void insertUpdateItem();
    void editItem();
};

#endif // GOODSENTRYEXIT_H
