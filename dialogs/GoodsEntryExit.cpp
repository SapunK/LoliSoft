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
static const QString Q_INSERT_NEW_DOC = "INSERT INTO documents (doc_type_id, doc_number, datetime) VALUES (%1, %2, '%3');";

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
    m_leRebatePct = new CustomIntLE(this);
    m_leTaxPct = new CustomIntLE(this);
    m_leMarginPct = new CustomIntLE(this);

    m_leEntryPrice = new CustomDoubleLE(this);
    m_leRebate = new CustomDoubleLE(this);
    m_leTax = new CustomDoubleLE(this);
    m_leMargin = new CustomDoubleLE(this);
    m_leDiscount = new CustomDoubleLE(this);
    m_leSalePrice = new CustomDoubleLE(this);
    m_lePriceDiff = new CustomDoubleLE(this);

    setTabOrder(m_leShoe, m_leEntryPrice);
    setTabOrder(m_leEntryPrice, m_leRebate);
    setTabOrder(m_leRebate, m_leRebatePct);

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

    m_vItemWidgets<<m_leShoe<<m_leEntryPrice<<m_leRebate<<m_leRebatePct<<m_leTax<<
                    m_leTaxPct<<m_leDiscount<<m_leSalePrice<<m_lePriceDiff<<
                    lbEntryPrice<<lbRebate<<lbRebatePct<<lbTax<<lbTaxPct<<
                    m_leMargin<<lbMargin<<m_leMarginPct<<lbMarginPct<<
                    lbDiscount<<lbSalePrice<<lbPriceDiff;

    showHideItemWidgets(true);

    mainLayout->addWidget(lbDocName,        0, 0);
    mainLayout->addWidget(m_cbDocName,      0, 1);

    mainLayout->addWidget(lbDate,           0, 2);
    mainLayout->addWidget(m_deDate,         0, 3);

    mainLayout->addWidget(m_pbNewDoc,       0, 4);

    mainLayout->addWidget(lbDocNumber,      0, 5);
    mainLayout->addWidget(m_leDocNumber,    0, 6);

    mainLayout->addWidget(m_pbNewItem,      1, 0);
    mainLayout->addWidget(m_pbEditItem,     1, 1);
    mainLayout->addWidget(m_pbDeleteItem,   1, 2);

    mainLayout->addWidget(lbShoe,           2, 0);
    mainLayout->addWidget(m_leShoe,         2, 1, 1, 3);

    mainLayout->addWidget(lbEntryPrice,     2, 4);
    mainLayout->addWidget(m_leEntryPrice,   2, 5);

    mainLayout->addWidget(lbRebate,         3, 0);
    mainLayout->addWidget(m_leRebate,       3, 1);

    mainLayout->addWidget(lbRebatePct,      3, 2);
    mainLayout->addWidget(m_leRebatePct,    3, 3);

    mainLayout->addWidget(lbTax,            3, 4);
    mainLayout->addWidget(m_leTax,          3, 5);

    mainLayout->addWidget(lbTaxPct,         3, 6);
    mainLayout->addWidget(m_leTaxPct,       3, 7);

    mainLayout->addWidget(lbMargin,         4, 0);
    mainLayout->addWidget(m_leMargin,       4, 1);

    mainLayout->addWidget(lbMarginPct,      4, 2);
    mainLayout->addWidget(m_leMarginPct,    4, 3);

    mainLayout->addWidget(lbDiscount,       4, 4);
    mainLayout->addWidget(m_leDiscount,     4, 5);

    mainLayout->addWidget(lbPriceDiff,      4, 6);
    mainLayout->addWidget(m_lePriceDiff,    4, 7);

    mainLayout->addWidget(lbSalePrice,      5, 4);
    mainLayout->addWidget(m_leSalePrice,    5, 5);
    mainLayout->addWidget(m_pbSaveItem,     5, 7);

    mainLayout->addWidget(m_pbSaveDoc,      6, 7);
    mainLayout->addWidget(m_table,    7, 0, 1, 7);

    setLayout(mainLayout);
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
    connect(m_pbNewItem, &QPushButton::clicked, this, [this]{
       showHideItemWidgets(false);
       m_leShoe->setFocus();
    });
}

void GoodsEntryExit::setDocNumber()
{
    if(m_leDocNumber->text().isEmpty()){
        QSqlQuery q;
        q.exec(Q_LAST_DOC_NUM);
        q.next();
        m_leDocNumber->setText(q.value(0).toString());
        QDateTime *currentDateTime = new QDateTime(m_deDate->date(), QTime::currentTime());
        if(!q.exec(Q_INSERT_NEW_DOC
                  .arg(m_cbDocName->model()->index(m_cbDocName->currentIndex(), 1).data().toInt())
                  .arg(m_leDocNumber->text().toInt())
                  .arg(currentDateTime->toString(Qt::ISODate))))
        {
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
            qCritical()<<"Unable to insert a new document query error: "<<q.lastError()<<" last query: "<<q.lastQuery();
            m_leDocNumber->clear();
            return;
        }
        m_pbNewItem->setEnabled(true);
    }
}
