#include "GoodsEntryExit.h"

#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QGridLayout>
#include <QLabel>

#include "appconsts.h"
#include "HelperFunctions.h"

namespace GoodsEntryExit_NS {
static const char* NEW_DOC = "New document";
static const char* INSERT_DOC = "Insert document";
static const char* DOC_NAME = "Document name";
static const char* DOC_NUMBER = "Document number";

}

using namespace GoodsEntryExit_NS;

GoodsEntryExit::GoodsEntryExit(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(HelperFunctions::desktopWidth() * 0.5, HelperFunctions::desktopHeight() * 0.4);
}

void GoodsEntryExit::setupForm()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    m_pbNewDoc = new QPushButton(this);
    m_pbInsertDoc = new QPushButton(this);
    m_pbNewItem = new QPushButton(this);
    m_pbEditItem = new QPushButton(this);
    m_pbDeleteItem = new QPushButton(this);

    m_leDocName = new QLineEdit(this);
    m_leDocNumber = new QLineEdit(this);
    m_leEntryPrice = new QLineEdit(this);
    m_leRebate = new QLineEdit(this);
    m_leRebatePct = new QLineEdit(this);
    m_leTax = new QLineEdit(this);
    m_leTaxPct = new QLineEdit(this);
    m_leDiscount = new QLineEdit(this);
    m_leSalePrice = new QLineEdit(this);
    m_lePriceDiff = new QLineEdit(this);

    m_deDate = new QDateEdit(this);

    QLabel *lbNewDoc = new QLabel(NEW_DOC, this);
    QLabel *lbInsertDoc = new QLabel(INSERT_DOC, this);
    QLabel *lbNewItem = new QLabel(NEW, this);
    QLabel *lbEditItem = new QLabel(EDIT, this);
    QLabel *lbDeleteItem = new QLabel(DELETE, this);
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


}
