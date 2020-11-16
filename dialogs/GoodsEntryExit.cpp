#include "GoodsEntryExit.h"

#include <QPushButton>
#include <QDateEdit>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QCompleter>
#include <QDebug>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QEvent>
#include <QKeyEvent>

#include "appconsts.h"
#include "HelperFunctions.h"
#include "custom_widgets/CustomTableView.h"
#include "custom_widgets/CustomDoubleLE.h"
#include "custom_widgets/CustomIntLE.h"

namespace GoodsEntryExit_NS {
static const char* NEW_DOC = "New document";
static const char* SAVE_DOC = "Save document";
static const char* DOC_TYPE = "Document type";
static const char* DOC_NUMBER = "Document nm";
static const char* NEW_ITEM = "New item";
static const char* EDIT_ITEM = "Edit item";
static const char* DELETE_ITEM = "Delete item";
static const char* SAVE_ITEM = "Save item";
static const char* MARGIN = "Margin";
static const char* MARGIN_PCT = "Margin %";
static const char* SHOE = "Shoe";
static const char* QUANTITY = "Quantity";
static const char* AFTER_REBATE = "After rebate";
static const char* DISCOUNT_PCT = "Discount %";
static const char* SUM_VALUE = "Sum value";
static const char* MSG_DELETE_QUESTION = "Are you sure you want to delete this item?";

static const char* Q_DOC_NAME_MODEL = "SELECT '' as doc_name, 0 as doc_type_id "
                                      "UNION "
                                      "SELECT doc_name, id as doc_type_id FROM doc_types "
                                      "ORDER BY doc_name;";
//TODO probably max doc number from certain doc type
static const char* Q_LAST_DOC_NUM = "SELECT COALESCE(MAX(doc_number), 0) + 1 FROM documents;";
static const char* Q_SHOE_COMPLETER = "SELECT m.model || ' ' || c.color || ' ' || s.code, s.code "
                                      "FROM shoes s "
                                      "INNER JOIN models m ON s.model = m.id "
                                      "INNER JOIN colors c ON s.color = c.id "
                                      "GROUP BY m.model, s.code, c.color;";
static const QString Q_INSERT_NEW_DOC = "INSERT INTO documents (doc_type_id, doc_number, datetime) VALUES (%1, %2, '%3') RETURNING id;";
static const QString Q_INSERT_ITEM = "INSERT INTO items (doc_id, shoe_id, size, quantity, entry_price, rebate, rebate_percentage, "
                                     "price_after_rebate, tax, tax_percentage, margin, margin_percentage, discount, "
                                     "discount_percentage, price_difference, sale_price, sum_value, datetime, updated) "
                                     "VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, '%18', '%19');";
static const QString Q_SELECT_ITEM = "SELECT m.model || ' ' || c.color || ' ' || s.code, i.size, i.quantity, i.entry_price, i.rebate, "
                                     "i.rebate_percentage, i.price_after_rebate, i.tax, i.tax_percentage, i.margin, i.margin_percentage, "
                                     "i.discount, i.discount_percentage, i.price_difference, i.sale_price "
                                     "FROM items i "
                                     "INNER JOIN shoes s on s.code = i.shoe_id "
                                     "INNER JOIN models m ON s.model = m.id "
                                     "INNER JOIN colors c ON s.color = c.id "
                                     "WHERE i.id = %1";
static const QString Q_UPDATE_ITEM = "UPDATE items SET shoe_id = %1, size = %2, quantity = %3, entry_price = %4, rebate = %5, "
                                     "rebate_percentage = %6, price_after_rebate = %7, tax = %8, tax_percentage = %9, margin = %10, "
                                     "margin_percentage = %11, discount = %12, discount_percentage = %13, price_difference = %14, "
                                     "sale_price = %15, sum_value = %16, updated = '%17' "
                                     "WHERE id = %18";
static const QString Q_DELETE_ITEM = "DELETE FROM items WHERE id = %1";
//TODO select only relevant columns
static const QString Q_MODEL_SELECT = "SELECT * FROM items WHERE doc_id = %1";

}

using namespace GoodsEntryExit_NS;

GoodsEntryExit::GoodsEntryExit(QWidget *parent)
    : QDialog(parent),
      m_itemId(-1)
{
    setFixedSize(HelperFunctions::desktopWidth() * 0.5, HelperFunctions::desktopHeight() * 0.4);
    setupForm();
}

void GoodsEntryExit::setupForm()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    m_pbNewDoc = new QPushButton(NEW_DOC, this);
    m_pbNewDoc->setDisabled(true);
    m_pbSaveDoc = new QPushButton(SAVE_DOC, this);
    m_pbNewItem = new QPushButton(NEW_ITEM, this);
    m_pbNewItem->setDisabled(true);
    m_pbEditItem = new QPushButton(EDIT_ITEM, this);
    m_pbEditItem->setDisabled(true);
    m_pbDeleteItem = new QPushButton(DELETE_ITEM, this);
    m_pbDeleteItem->setDisabled(true);
    m_pbSaveItem = new QPushButton(SAVE_ITEM, this);

    m_leDocNumber = new QLineEdit(this);
    m_leDocNumber->setDisabled(true);
    m_leDocValue = new CustomDoubleLE(this);
    m_leDocValue->setDisabled(true);

    m_cbShoe = new QComboBox(this);
    m_cbShoe->setAccessibleName(SHOE);
    m_leSize = new CustomIntLE(this);
    m_leSize->setAccessibleName(SIZE);
    m_leQuantity = new CustomIntLE(this);
    m_leQuantity->setAccessibleName(QUANTITY);

    m_leEntryPrice = new CustomDoubleLE(this);
    m_leEntryPrice->setAccessibleName(ENTRY_PRICE);
    m_leRebate = new CustomDoubleLE(this);
    m_leRebate->setAccessibleName(REBATE);
    //TODO probably make a combo for tax or fixed 18%?
    m_leTax = new CustomDoubleLE(this);
    m_leTax->setDisabled(true);
    m_leTax->setAccessibleName(TAX);
    m_leMargin = new CustomDoubleLE(this);
    m_leMargin->setAccessibleName(MARGIN);
    m_leDiscount = new CustomDoubleLE(this);
    m_leDiscount->setAccessibleName(DISCOUNT);
    m_leDiscountPct = new CustomDoubleLE(this);
    m_leDiscountPct->setAccessibleName(DISCOUNT_PCT);
    m_leSalePrice = new CustomDoubleLE(this);
    m_leSalePrice->setAccessibleName(SALE_PRICE);
    m_lePriceDiff = new CustomDoubleLE(this);
    m_lePriceDiff->setAccessibleName(PRICE_DIFF);
    m_lePriceDiff->setDisabled(true);
    m_leRebatePct = new CustomDoubleLE(this);
    m_leRebatePct->setAccessibleName(REBATE_PCT);
    m_leTaxPct = new CustomDoubleLE(this);
    m_leTaxPct->setAccessibleName(TAX_PCT);
    m_leMarginPct = new CustomDoubleLE(this);
    m_leMarginPct->setAccessibleName(MARGIN_PCT);
    m_lePrcAfterRebate = new CustomDoubleLE(this);
    m_lePrcAfterRebate->setAccessibleName(AFTER_REBATE);

    QSqlQueryModel *completerModel = new QSqlQueryModel(this);
    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
    completerModel->setQuery(Q_SHOE_COMPLETER);
    sortModel->setSourceModel(completerModel);
    m_cbShoe->setEditable(true);
    m_cbShoe->setModel(sortModel);
    m_cbShoe->setModelColumn(0);
    m_cbShoe->completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    m_cbShoe->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    m_cbShoe->completer()->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    m_cbShoe->setCurrentIndex(-1);

    connect(m_cbShoe->lineEdit(), &QLineEdit::textEdited, this, [this, sortModel](QString text){
        if(text.isEmpty()){
            m_cbShoe->setCurrentIndex(-1);
            return;
        }
        sortModel->setFilterWildcard("*" + text + "*");
    });

    m_deDate = new QDateEdit(QDate::currentDate(), this);
    m_deDate->setCalendarPopup(true);
    m_deDate->setDisplayFormat(DATE_FORMAT);

    m_cbDocType = new QComboBox(this);
    QSqlQueryModel *docNameModel = new QSqlQueryModel(this);
    docNameModel->setQuery(Q_DOC_NAME_MODEL);
    m_cbDocType->setModel(docNameModel);
    m_cbDocType->setEditable(false);

    connect(m_cbDocType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]{
        m_pbNewDoc->setDisabled(m_cbDocType->currentText().isEmpty());
    });

    m_table = new CustomTableView(this);
    m_model = new QSqlQueryModel(this);
    m_table->setModel(m_model);

    QLabel *lbDocType = new QLabel(DOC_TYPE, this);
    QLabel *lbDocNumber = new QLabel(DOC_NUMBER, this);
    QLabel *lbShoe = new QLabel(SHOE, this);
    QLabel *lbSize = new QLabel(SIZE, this);
    QLabel *lbQuantity = new QLabel(QUANTITY, this);
    QLabel *lbEntryPrice = new QLabel(ENTRY_PRICE, this);
    QLabel *lbRebate = new QLabel(REBATE, this);
    QLabel *lbRebatePct = new QLabel(REBATE_PCT, this);
    QLabel *lbTax = new QLabel(TAX, this);
    QLabel *lbTaxPct = new QLabel(TAX_PCT, this);
    QLabel *lbMargin = new QLabel(MARGIN, this);
    QLabel *lbMarginPct = new QLabel(MARGIN_PCT, this);
    QLabel *lbDiscount = new QLabel(DISCOUNT, this);
    QLabel *lbDiscountPct = new QLabel(DISCOUNT_PCT, this);
    QLabel *lbSalePrice = new QLabel(SALE_PRICE, this);
    QLabel *lbPriceDiff = new QLabel(PRICE_DIFF, this);
    QLabel *lbDate = new QLabel(DATE, this);
    QLabel *lbAfterRebate = new QLabel(AFTER_REBATE, this);
    QLabel *lbSumValue = new QLabel(SUM_VALUE, this);

    //We first add line edits to the vector so we can connect them to a slot in connectWidgets()
    m_vItemWidgets<<m_leEntryPrice<<m_leRebate<<m_leRebatePct<<m_lePrcAfterRebate<<
                    m_leTax<<m_leTaxPct<<m_leMargin<<m_leMarginPct<<
                    m_leDiscount<<m_leDiscountPct<<m_lePriceDiff<<m_leSalePrice;

    connectWidgets();

    //Then we add the other widgets to the vector for show/hide method
    m_vItemWidgets<<m_pbSaveItem<<m_cbShoe<<m_leSize<<m_leQuantity<<
                    lbEntryPrice<<lbRebate<<lbRebatePct<<lbTax<<lbTaxPct<<
                    lbDiscount<<lbSalePrice<<lbPriceDiff<<lbMarginPct<<
                    lbMargin<<lbSize<<lbQuantity<<lbAfterRebate<<lbShoe<<lbDiscountPct;

    QVector<QWidget*> vTabOrderWidgets;
    vTabOrderWidgets<<m_cbShoe<<m_leSize<<m_leQuantity<<
                      m_leEntryPrice<<m_leRebate<<m_leRebatePct<<m_lePrcAfterRebate<<
                      m_leTax<<m_leTaxPct<<m_leMargin<<m_leMarginPct<<
                      m_leDiscount<<m_leDiscountPct<<m_lePriceDiff<<m_leSalePrice<<m_pbSaveItem;

    HelperFunctions::setTabOrder(this, vTabOrderWidgets);

    showHideItemWidgets(true);

    int lColumns = -1;

    mainLayout->addWidget(lbDocType,        0, ++lColumns);
    mainLayout->addWidget(m_cbDocType,      0, ++lColumns);

    mainLayout->addWidget(lbDate,           0, ++lColumns);
    mainLayout->addWidget(m_deDate,         0, ++lColumns);

    mainLayout->addWidget(m_pbNewDoc,       0, ++lColumns);

    mainLayout->addWidget(lbDocNumber,      0, ++lColumns);
    mainLayout->addWidget(m_leDocNumber,    0, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(m_pbNewItem,      1, ++lColumns);
    mainLayout->addWidget(m_pbEditItem,     1, ++lColumns);
    mainLayout->addWidget(m_pbDeleteItem,   1, ++lColumns);
    mainLayout->addWidget(lbSumValue,       1, 5);
    mainLayout->addWidget(m_leDocValue,     1, 6);

    lColumns = -1;

    mainLayout->addWidget(lbShoe,           2, 0);
    mainLayout->addWidget(m_cbShoe,         2, 1, 1, 3);

    mainLayout->addWidget(lbSize,           2, 4);
    mainLayout->addWidget(m_leSize,         2, 5);

    mainLayout->addWidget(lbQuantity,       2, 6);
    mainLayout->addWidget(m_leQuantity,     2, 7);

    mainLayout->addWidget(lbEntryPrice,       3, ++lColumns);
    mainLayout->addWidget(m_leEntryPrice,     3, ++lColumns);

    mainLayout->addWidget(lbRebate,           3, ++lColumns);
    mainLayout->addWidget(m_leRebate,         3, ++lColumns);

    mainLayout->addWidget(lbRebatePct,        3, ++lColumns);
    mainLayout->addWidget(m_leRebatePct,      3, ++lColumns);

    mainLayout->addWidget(lbAfterRebate,      3, ++lColumns);
    mainLayout->addWidget(m_lePrcAfterRebate, 3, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(lbTax,            4, ++lColumns);
    mainLayout->addWidget(m_leTax,          4, ++lColumns);

    mainLayout->addWidget(lbTaxPct,         4, ++lColumns);
    mainLayout->addWidget(m_leTaxPct,       4, ++lColumns);

    mainLayout->addWidget(lbMargin,         4, ++lColumns);
    mainLayout->addWidget(m_leMargin,       4, ++lColumns);

    mainLayout->addWidget(lbMarginPct,      4, ++lColumns);
    mainLayout->addWidget(m_leMarginPct,    4, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(lbDiscount,          5, ++lColumns);
    mainLayout->addWidget(m_leDiscount,        5, ++lColumns);

    mainLayout->addWidget(lbDiscountPct,       5, ++lColumns);
    mainLayout->addWidget(m_leDiscountPct,     5, ++lColumns);

    mainLayout->addWidget(lbPriceDiff,         5, ++lColumns);
    mainLayout->addWidget(m_lePriceDiff,       5, ++lColumns);

    mainLayout->addWidget(lbSalePrice,         5, ++lColumns);
    mainLayout->addWidget(m_leSalePrice,       5, ++lColumns);

    mainLayout->addWidget(m_pbSaveItem,     6, 7);

    mainLayout->addWidget(m_pbSaveDoc,      7, 7);
    mainLayout->addWidget(m_table,    8, 0, 1, 7);
}

void GoodsEntryExit::showHideItemWidgets(bool hide)
{
    for(int i = 0 ; i < m_vItemWidgets.size() ; i++)
    {
        m_vItemWidgets.at(i)->setHidden(hide);
    }

    if(hide)
        m_cbShoe->setCurrentIndex(-1);
}

void GoodsEntryExit::connectWidgets()
{
    connect(m_pbNewDoc, &QPushButton::clicked, this, &GoodsEntryExit::setDocNumber);
    connect(m_pbSaveItem, &QPushButton::clicked, this, &GoodsEntryExit::insertUpdateItem);
    connect(m_pbNewItem, &QPushButton::clicked, this, [this]{
       showHideItemWidgets(false);
       m_cbShoe->setFocus();
    });
    connect(m_pbEditItem, &QPushButton::clicked, this, &GoodsEntryExit::editItem);
    connect(m_pbDeleteItem, &QPushButton::clicked, this, &GoodsEntryExit::deleteItem);
    connect(m_table->selectionModel(), &QItemSelectionModel::currentRowChanged, this, [this]{
        if(m_table->currentIndex().isValid()){
            m_pbEditItem->setEnabled(true);
            m_pbDeleteItem->setEnabled(true);
        }
    });


    for(int i = 0 ; i < m_vItemWidgets.size() ; i++)
    {
        connect(static_cast<QLineEdit*>(m_vItemWidgets.at(i)), &QLineEdit::editingFinished, this, &GoodsEntryExit::updateFields);
    }
}

bool GoodsEntryExit::validateItem()
{
    for(int i = 0 ; i < m_vItemWidgets.size() ; i++)
    {
        if(m_vItemWidgets.at(i)->metaObject()->className() == QString("QLineEdit")
                && !m_vItemWidgets.at(i)->accessibleName().isEmpty()
                && static_cast<QLineEdit*>(m_vItemWidgets.at(i))->text().isEmpty())
        {
            QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + ": " + m_vItemWidgets.at(i)->accessibleName());
            return false;
        }
    }
    return true;
}

void GoodsEntryExit::clearItemFields()
{
    for(int i = 0 ; i < m_vItemWidgets.size() ; i++)
    {
        if(m_vItemWidgets.at(i)->metaObject()->className() == QString("QLineEdit"))
        {
            static_cast<QLineEdit*>(m_vItemWidgets.at(i))->clear();
        }
    }
}

void GoodsEntryExit::updateFields()
{
    double entryPrice = m_leEntryPrice->value();
    double rebatePct = m_leRebatePct->value();
    double rebate = m_leRebate->value();
    double taxPct = m_leTaxPct->value() / 100;

    if(sender() == m_leRebate){
        rebatePct = (rebate / entryPrice) * 100;
    } else {
        rebate = (rebatePct / 100) * entryPrice;
    }

    double prcAfterRebate = taxPct != 0 ? (entryPrice - rebate)  + (entryPrice - rebate) * taxPct
                                        : entryPrice - rebate;
    double tax = prcAfterRebate * (m_leTaxPct->value() / 100);

    double marginPct = m_leMarginPct->value();
    double margin = m_leMargin->value();

    if(sender() == m_leMargin){
        marginPct = (margin / prcAfterRebate) * 100;
    } else {
        margin = (marginPct / 100) * prcAfterRebate;
    }

    double salePrice = prcAfterRebate + margin;

    double discount = m_leDiscount->value();
    double discountPct = m_leDiscountPct->value();

    if(sender() == m_leDiscount){
        discountPct = (discount / salePrice) * 100;
    } else {
        discount = (discountPct / 100) * salePrice;
    }

    salePrice = salePrice - discount;

    if(taxPct > 0)
        salePrice = salePrice + salePrice * taxPct;

    double priceDifference = salePrice - prcAfterRebate;

    m_leRebate->setValue(rebate);
    m_leRebatePct->setValue(rebatePct);
    m_lePrcAfterRebate->setValue(prcAfterRebate);
    m_leTax->setValue(tax);
    m_leMargin->setValue(margin);
    m_leMarginPct->setValue(marginPct);
    m_leDiscount->setValue(discount);
    m_leDiscountPct->setValue(discountPct);
    m_lePriceDiff->setValue(priceDifference);
    m_leSalePrice->setValue(salePrice);

}

void GoodsEntryExit::setDocNumber()
{
    if(m_leDocNumber->text().isEmpty()){
        QSqlQuery q;
        q.exec(Q_LAST_DOC_NUM);
        q.next();
        m_leDocNumber->setText(q.value(0).toString());
        m_docId = q.value(0).toInt();
        if(!q.exec(Q_INSERT_NEW_DOC
                  .arg(m_cbDocType->model()->index(m_cbDocType->currentIndex(), 1).data().toInt())
                  .arg(m_docId)
                  .arg(QDateTime::currentDateTime().toString(Qt::ISODate))))
        {
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
            qCritical()<<"Unable to insert a new document query error: "<<q.lastError()<<" last query: "<<q.lastQuery();
            m_leDocNumber->clear();
            return;
        }
        m_pbNewItem->setEnabled(true);
        m_pbNewDoc->setDisabled(true);
        m_cbDocType->setDisabled(true);

    }
}

void GoodsEntryExit::insertUpdateItem()
{
    if(!validateItem())
        return;

    QSqlQuery q;
    int shoeId = m_cbShoe->model()->index(m_cbShoe->currentIndex(), 1).data().toInt();
    int size = m_leSize->value();
    int quantity = m_leQuantity->value();
    double entryPrice = m_leEntryPrice->value();
    double rebate = m_leRebate->value();
    double rebatePct = m_leRebatePct->value();
    double priceAfterRebate = m_lePrcAfterRebate->value();
    double tax = m_leTax->value();
    double taxPct = m_leTaxPct->value();
    double margin = m_leMargin->value();
    double marginPct = m_leMarginPct->value();
    double discount = m_leDiscount->value();
    double discountPct = m_leDiscountPct->value();
    double priceDiff = m_lePriceDiff->value();
    double salePrice = m_leSalePrice->value();
    double sumValue = salePrice * quantity;
    QString currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);

    if(m_itemId == -1){
        q.prepare(Q_INSERT_ITEM.arg(m_docId).arg(shoeId).arg(size).arg(quantity).arg(entryPrice).arg(rebate)
                  .arg(rebatePct).arg(priceAfterRebate).arg(tax).arg(taxPct).arg(margin).arg(marginPct).arg(discount)
                  .arg(discountPct).arg(priceDiff).arg(salePrice).arg(sumValue).arg(currentDateTime).arg(currentDateTime));
    } else {
        q.prepare(Q_UPDATE_ITEM.arg(shoeId).arg(size).arg(quantity).arg(entryPrice).arg(rebate)
                  .arg(rebatePct).arg(priceAfterRebate).arg(tax).arg(taxPct).arg(margin).arg(marginPct).arg(discount)
                  .arg(discountPct).arg(priceDiff).arg(salePrice).arg(sumValue).arg(currentDateTime).arg(m_itemId));
    }

    if(q.exec())
    {
        m_itemId = -1;
        m_cbShoe->setCurrentIndex(-1);
        static_cast<QSortFilterProxyModel*>(m_cbShoe->model())->setFilterWildcard("*");
        clearItemFields();
        m_model->setQuery(Q_MODEL_SELECT.arg(m_docId));
        double docSumValue = 0;
        for(int i = 0 ; i < m_model->rowCount() ; i++)
        {
            //TODO change 17 to enum
            docSumValue += m_model->index(i, 17).data().toDouble();
        }
        m_leDocValue->setValue(docSumValue);
        showHideItemWidgets(true);
        m_pbNewItem->setFocus();
    } else {
        qWarning()<<"query error: "<<q.lastError()<<"\nlast query: "<<q.lastQuery();
    }
}

void GoodsEntryExit::editItem()
{
    m_itemId = m_model->index(m_table->currentIndex().row(), 0).data().toInt();
    QSqlQuery q;
    if(q.exec(Q_SELECT_ITEM.arg(m_itemId)) && q.next())
    {
        m_cbShoe->setCurrentIndex(m_cbShoe->findText(q.value(EEditItem::shoe).toString()));
        m_leSize->setValue(q.value(EEditItem::size).toInt());
        m_leQuantity->setValue(q.value(EEditItem::quantity).toInt());
        m_leEntryPrice->setValue(q.value(EEditItem::entryPrice).toDouble());
        m_leRebate->setValue(q.value(EEditItem::rebate).toDouble());
        m_leRebatePct->setValue(q.value(EEditItem::rebatePct).toDouble());
        m_lePrcAfterRebate->setValue(q.value(EEditItem::priceAfterRebate).toDouble());
        m_leTax->setValue(q.value(EEditItem::tax).toDouble());
        m_leTaxPct->setValue(q.value(EEditItem::taxPct).toDouble());
        m_leMargin->setValue(q.value(EEditItem::margin).toDouble());
        m_leMarginPct->setValue(q.value(EEditItem::marginPct).toDouble());
        m_leDiscount->setValue(q.value(EEditItem::discount).toDouble());
        m_leDiscountPct->setValue(q.value(EEditItem::discountPct).toDouble());
        m_lePriceDiff->setValue(q.value(EEditItem::priceDiff).toDouble());
        m_leSalePrice->setValue(q.value(EEditItem::salePrice).toDouble());
        showHideItemWidgets(false);
    }

}

void GoodsEntryExit::deleteItem()
{
    if(QMessageBox::Yes == QMessageBox::question(this, DELETE_ITEM, MSG_DELETE_QUESTION))
    {
        //TODO use enum for column
        int itemId = m_model->index(m_table->currentIndex().row(), 0).data().toInt();
        QSqlQuery q;
        if(q.exec(Q_DELETE_ITEM.arg(itemId)))
        {
            m_model->setQuery(m_model->query().lastQuery());
        } else {
            qCritical()<<"Last query: "<<q.lastQuery()<<"\nLast error: "<<q.lastError();
        }
    }
}

bool GoodsEntryExit::event(QEvent *event){
    if(event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Escape){
            if(!m_cbShoe->isHidden()){
                clearItemFields();
                showHideItemWidgets(true);
                return true;
            }
            //TODO else ask if the user is sure that he wants to close the dialog
        }
    }
    return QWidget::event(event);
}
