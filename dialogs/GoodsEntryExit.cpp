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

#include "appconsts.h"
#include "HelperFunctions.h"
#include "custom_widgets/CustomTableView.h"
#include "custom_widgets/CustomDoubleLE.h"
#include "custom_widgets/CustomIntLE.h"

namespace GoodsEntryExit_NS {
static const char* NEW_DOC = "New document";
static const char* SAVE_DOC = "Save document";
static const char* DOC_NAME = "Document name";
static const char* DOC_NUMBER = "Document nm";
static const char* NEW_ITEM = "New item";
static const char* EDIT_ITEM = "Edit item";
static const char* DELETE_ITEM = "Delete item";
static const char* SAVE_ITEM = "Save item";
static const char* MARGIN = "Margin";
static const char* MARGIN_PCT = "Margin %";
static const char* SHOE = "Shoe";
static const char* QUANTITY = "Quantity";

static const char* Q_DOC_NAME_MODEL = "SELECT '' as doc_name, 0 as doc_type_id "
                                      "UNION "
                                      "SELECT doc_name, id as doc_type_id FROM doc_types "
                                      "ORDER BY doc_name;";
//TODO probably max doc number from certain doc type
static const char* Q_LAST_DOC_NUM = "SELECT COALESCE(MAX(doc_number), 1) + 1 FROM documents;";
static const char* Q_SHOE_COMPLETER = "SELECT m.model || ' ' || c.color || ' ' || s.code "
                                      "FROM shoes s "
                                      "INNER JOIN models m ON s.model = m.id "
                                      "INNER JOIN colors c ON s.color = c.id "
                                      "GROUP BY m.model, s.code, c.color;";
static const QString Q_INSERT_NEW_DOC = "INSERT INTO documents (doc_type_id, doc_number, datetime) VALUES (%1, %2, '%3') RETURNING id;";
static const QString Q_INSERT_ITEM = "INSERT INTO items (doc_id, shoe_id, size, entry_price, rebate, rebate_percentage, "
                                     "tax, tax_percentage, sale_price, discount, price_difference, quantity, datetime, updated) "
                                     "VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, '%13', '%14');";
//TODO select only relevant columns
static const QString Q_MODEL_SELECT = "SELECT * FROM items WHERE doc_id = %1";

}

using namespace GoodsEntryExit_NS;

GoodsEntryExit::GoodsEntryExit(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(HelperFunctions::desktopWidth() * 0.5, HelperFunctions::desktopHeight() * 0.4);
    setupForm();
    connectWidgets();
}

void GoodsEntryExit::setupForm()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    m_pbNewDoc = new QPushButton(NEW_DOC, this);
    m_pbSaveDoc = new QPushButton(SAVE_DOC, this);
    m_pbNewItem = new QPushButton(NEW_ITEM, this);
    m_pbNewItem->setDisabled(true);
    m_pbEditItem = new QPushButton(EDIT_ITEM, this);
    m_pbEditItem->setDisabled(true);
    m_pbDeleteItem = new QPushButton(DELETE_ITEM, this);
    m_pbDeleteItem->setDisabled(true);
    m_pbSaveItem = new QPushButton(SAVE_ITEM, this);

    m_leShoe = new QLineEdit(this);
    m_leDocNumber = new QLineEdit(this);
    m_leDocNumber->setDisabled(true);

    m_leSize = new CustomIntLE(this);
    m_leQuantity = new CustomIntLE(this);

    m_leEntryPrice = new CustomDoubleLE(this);
    m_leRebate = new CustomDoubleLE(this);
    m_leTax = new CustomDoubleLE(this);
    m_leMargin = new CustomDoubleLE(this);
    m_leDiscount = new CustomDoubleLE(this);
    m_leSalePrice = new CustomDoubleLE(this);
    m_lePriceDiff = new CustomDoubleLE(this);
    m_leRebatePct = new CustomDoubleLE(this);
    m_leTaxPct = new CustomDoubleLE(this);
    m_leMarginPct = new CustomDoubleLE(this);

    QStringList completerList;
    QSqlQuery qCompleter;
    qCompleter.exec(Q_SHOE_COMPLETER);
    while(qCompleter.next())
    {
        completerList.append(qCompleter.value(0).toString());
    }
    QCompleter *shoeCompleter = new QCompleter(completerList, this);
    shoeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    shoeCompleter->setFilterMode(Qt::MatchContains);
    m_leShoe->setCompleter(shoeCompleter);

    m_deDate = new QDateEdit(QDate::currentDate(), this);
    m_deDate->setCalendarPopup(true);
    m_deDate->setDisplayFormat(DATE_FORMAT);

    m_cbDocName = new QComboBox(this);
    QSqlQueryModel *docNameModel = new QSqlQueryModel(this);
    docNameModel->setQuery(Q_DOC_NAME_MODEL);
    m_cbDocName->setModel(docNameModel);
    m_cbDocName->setEditable(false);

    m_table = new CustomTableView(this);
    m_model = new QSqlQueryModel(this);
    m_table->setModel(m_model);

    QLabel *lbDocName = new QLabel(DOC_NAME, this);
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
    QLabel *lbSalePrice = new QLabel(SALE_PRICE, this);
    QLabel *lbPriceDiff = new QLabel(PRICE_DIFF, this);
    QLabel *lbDate = new QLabel(DATE, this);

    m_vItemWidgets<<m_leShoe<<m_leSize<<m_leQuantity<<m_leEntryPrice<<m_leRebate<<m_leRebatePct<<
                    m_leTax<<m_leTaxPct<<m_leMargin<<m_leMarginPct<<
                    m_leDiscount<<m_lePriceDiff<<m_leSalePrice<<m_pbSaveItem;

    HelperFunctions::setTabOrder(this, m_vItemWidgets);

    m_vItemWidgets<<lbEntryPrice<<lbRebate<<lbRebatePct<<lbTax<<lbTaxPct<<
                    lbDiscount<<lbSalePrice<<lbPriceDiff<<lbMarginPct<<lbMargin<<lbSize<<lbQuantity;

    showHideItemWidgets(true);

    int lColumns = -1;

    mainLayout->addWidget(lbDocName,        0, ++lColumns);
    mainLayout->addWidget(m_cbDocName,      0, ++lColumns);

    mainLayout->addWidget(lbDate,           0, ++lColumns);
    mainLayout->addWidget(m_deDate,         0, ++lColumns);

    mainLayout->addWidget(m_pbNewDoc,       0, ++lColumns);

    mainLayout->addWidget(lbDocNumber,      0, ++lColumns);
    mainLayout->addWidget(m_leDocNumber,    0, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(m_pbNewItem,      1, ++lColumns);
    mainLayout->addWidget(m_pbEditItem,     1, ++lColumns);
    mainLayout->addWidget(m_pbDeleteItem,   1, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(lbShoe,           2, 0);
    mainLayout->addWidget(m_leShoe,         2, 1, 1, 3);

    mainLayout->addWidget(lbSize,           2, 4);
    mainLayout->addWidget(m_leSize,         2, 5);

    mainLayout->addWidget(lbQuantity,       2, 6);
    mainLayout->addWidget(m_leQuantity,     2, 7);

    mainLayout->addWidget(lbEntryPrice,     3, ++lColumns);
    mainLayout->addWidget(m_leEntryPrice,   3, ++lColumns);

    mainLayout->addWidget(lbRebate,         3, ++lColumns);
    mainLayout->addWidget(m_leRebate,       3, ++lColumns);

    mainLayout->addWidget(lbRebatePct,      3, ++lColumns);
    mainLayout->addWidget(m_leRebatePct,    3, ++lColumns);

    mainLayout->addWidget(lbTax,            3, ++lColumns);
    mainLayout->addWidget(m_leTax,          3, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(lbTaxPct,         4, ++lColumns);
    mainLayout->addWidget(m_leTaxPct,       4, ++lColumns);

    mainLayout->addWidget(lbMargin,         4, ++lColumns);
    mainLayout->addWidget(m_leMargin,       4, ++lColumns);

    mainLayout->addWidget(lbMarginPct,      4, ++lColumns);
    mainLayout->addWidget(m_leMarginPct,    4, ++lColumns);

    mainLayout->addWidget(lbDiscount,       4, ++lColumns);
    mainLayout->addWidget(m_leDiscount,     4, ++lColumns);

    lColumns = -1;

    mainLayout->addWidget(lbPriceDiff,      5, ++lColumns);
    mainLayout->addWidget(m_lePriceDiff,    5, ++lColumns);

    mainLayout->addWidget(lbSalePrice,      5, ++lColumns);
    mainLayout->addWidget(m_leSalePrice,    5, ++lColumns);
    mainLayout->addWidget(m_pbSaveItem,     5, 7);

    mainLayout->addWidget(m_pbSaveDoc,      6, 7);
    mainLayout->addWidget(m_table,    7, 0, 1, 7);
}

void GoodsEntryExit::showHideItemWidgets(bool hide)
{
    for(int i = 0 ; i < m_vItemWidgets.size() ; i++)
    {
        m_vItemWidgets.at(i)->setHidden(hide);
    }
}

void GoodsEntryExit::connectWidgets()
{
    connect(m_pbNewDoc, &QPushButton::clicked, this, &GoodsEntryExit::setDocNumber);
    connect(m_pbSaveItem, &QPushButton::clicked, this, &GoodsEntryExit::insertItem);
    connect(m_pbNewItem, &QPushButton::clicked, this, [this]{
       showHideItemWidgets(false);
       m_leShoe->setFocus();
    });
}

bool GoodsEntryExit::validateItem()
{
    if(m_leShoe->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(SHOE));
        return false;
    }
    
    if(m_leRebatePct->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + ": " + QString(REBATE_PCT));
        return false;
    }
    
    if(m_leTaxPct->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(TAX_PCT));
        return false;
    }
    
    if(m_leMarginPct->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(MARGIN_PCT));
        return false;
    }
    
    if(m_leEntryPrice->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(ENTRY_PRICE));
        return false;
    }
    
    if(m_leRebate->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(REBATE));
        return false;
    }
    if(m_leTax->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(TAX));
        return false;
    }
    if(m_leMargin->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(MARGIN));
        return false;
    }
    if(m_leDiscount->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(DISCOUNT));
        return false;
    }
    if(m_leSalePrice->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(SALE_PRICE));
        return false;
    }
    if(m_lePriceDiff->text().isEmpty())
    {
        QMessageBox::warning(this, MSG_ERROR, QString(EMPTY_FIELD) + QString(PRICE_DIFF));
        return false;
    }
    
    return true;
}

void GoodsEntryExit::clearItemFields()
{
    m_leShoe->clear();
    m_leSize->clear();
    m_leQuantity->clear();
    m_leEntryPrice->clear();
    m_leRebate->clear();
    m_leRebatePct->clear();
    m_leTax->clear();
    m_leTaxPct->clear();
    m_leMargin->clear();
    m_leMarginPct->clear();
    m_leDiscount->clear();
    m_lePriceDiff->clear();
    m_leSalePrice->clear();
}

void GoodsEntryExit::setDocNumber()
{
    if(m_leDocNumber->text().isEmpty()){
        QSqlQuery q;
        q.exec(Q_LAST_DOC_NUM);
        q.next();
        m_leDocNumber->setText(q.value(0).toString());
        if(!q.exec(Q_INSERT_NEW_DOC
                  .arg(m_cbDocName->model()->index(m_cbDocName->currentIndex(), 1).data().toInt())
                  .arg(m_leDocNumber->text().toInt())
                  .arg(QDateTime::currentDateTime().toString(Qt::ISODate))))
        {
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
            qCritical()<<"Unable to insert a new document query error: "<<q.lastError()<<" last query: "<<q.lastQuery();
            m_leDocNumber->clear();
            return;
        }
        m_pbNewItem->setEnabled(true);
        m_docId = q.value(0).toInt();
    }
}

void GoodsEntryExit::insertItem()
{
    if(!validateItem())
        return;

    QSqlQuery q;
    int shoeId = m_leShoe->completer()->model()->index(m_leShoe->completer()->currentRow(), 1).data().toInt();
    int size = m_leSize->value();
    int quantity = m_leQuantity->value();
    double entryPrice = m_leEntryPrice->value();
    double rebate = m_leRebate->value();
    double rebatePct = m_leRebatePct->value();
    double tax = m_leTax->value();
    double taxPct = m_leTaxPct->value();
    double salePrice = m_leSalePrice->value();
    double discount = m_leDiscount->value();
    double priceDiff = m_lePriceDiff->value();
    QString currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);

    q.prepare(Q_INSERT_ITEM.arg(m_docId).arg(shoeId).arg(size).arg(entryPrice).arg(rebate)
              .arg(rebatePct).arg(tax).arg(taxPct).arg(salePrice).arg(discount).arg(priceDiff)
              .arg(quantity).arg(currentDateTime).arg(currentDateTime));
    if(q.exec())
    {
       clearItemFields();
       m_model->setQuery(Q_MODEL_SELECT.arg(m_docId));
    }
}
