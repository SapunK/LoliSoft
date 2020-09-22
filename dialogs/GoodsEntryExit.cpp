#include "GoodsEntryExit.h"

#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QCompleter>

#include "appconsts.h"
#include "HelperFunctions.h"
#include "custom_widgets/CustomTableView.h"

namespace GoodsEntryExit_NS {
static const char* NEW_DOC = "New document";
static const char* SAVE_DOC = "Save document";
static const char* DOC_NAME = "Document name";
static const char* DOC_NUMBER = "Document number";
static const char* NEW_ITEM = "New item";
static const char* EDIT_ITEM = "Edit item";
static const char* DELETE_ITEM = "Delete item";

static const char* Q_DOC_NAME_MODEL = "SELECT '' as doc_name "
                                      "UNION "
                                      "SELECT doc_name FROM doc_types "
                                      "ORDER BY doc_name;";

static const char* Q_LAST_DOC_NUM = "SELECT COALESCE(MAX(doc_number), 1) FROM documents;";
static const char* Q_SHOE_COMPLETER = "SELECT m.model || ' ' || s.code "
                                      "FROM shoes s "
                                      "INNER JOIN models m ON s.model = m.id "
                                      "GROUP BY m.model, s.code;";

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

    m_leDocNumber = new QLineEdit(this);
    m_leDocNumber->setDisabled(true);
    m_leShoe = new QLineEdit(this);
    m_leEntryPrice = new QLineEdit(this);
    m_leRebate = new QLineEdit(this);
    m_leRebatePct = new QLineEdit(this);
    m_leTax = new QLineEdit(this);
    m_leTaxPct = new QLineEdit(this);
    m_leDiscount = new QLineEdit(this);
    m_leSalePrice = new QLineEdit(this);
    m_lePriceDiff = new QLineEdit(this);

    QStringList completerList;
    QSqlQuery qCompleter;
    qCompleter.exec(Q_SHOE_COMPLETER);
    while(qCompleter.next())
    {
        completerList.append(qCompleter.value(0).toString());
    }
    QCompleter *shoeCompleter = new QCompleter(completerList, this);
    shoeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
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
    QLabel *lbEntryPrice = new QLabel(ENTRY_PRICE, this);
    QLabel *lbRebate = new QLabel(REBATE, this);
    QLabel *lbRebatePct = new QLabel(REBATE_PCT, this);
    QLabel *lbTax = new QLabel(TAX, this);
    QLabel *lbTaxPct = new QLabel(TAX_PCT, this);
    QLabel *lbDiscount = new QLabel(DISCOUNT, this);
    QLabel *lbSalePrice = new QLabel(SALE_PRICE, this);
    QLabel *lbPriceDiff = new QLabel(PRICE_DIFF, this);
    QLabel *lbDate = new QLabel(DATE, this);

    m_vItemWidgets<<m_leShoe<<m_leEntryPrice<<m_leRebate<<m_leRebatePct<<m_leTax<<
                    m_leTaxPct<<m_leDiscount<<m_leSalePrice<<m_lePriceDiff<<
                    lbEntryPrice<<lbRebate<<lbRebatePct<<lbTax<<lbTaxPct<<
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

    mainLayout->addWidget(m_leShoe,         2, 0);

    mainLayout->addWidget(lbEntryPrice,     3, 0);
    mainLayout->addWidget(m_leEntryPrice,   3, 1);

    mainLayout->addWidget(lbRebate,         3, 2);
    mainLayout->addWidget(m_leRebate,       3, 3);

    mainLayout->addWidget(m_pbSaveDoc,      4, 7);
    mainLayout->addWidget(m_table, 5, 0, 1, 7);

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
    });
}

void GoodsEntryExit::setDocNumber()
{
    //TODO insert a new document here
    if(m_leDocNumber->text().isEmpty()){
        QSqlQuery q;
        q.exec(Q_LAST_DOC_NUM);
        q.next();
        m_leDocNumber->setText(q.value(0).toString());
        m_pbNewItem->setEnabled(true);
    }
}
