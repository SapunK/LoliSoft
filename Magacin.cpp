#include "Magacin.h"

Magacin::Magacin(QDialog *parent) : QDialog(parent)
{
    setupForm();
    setFixedSize(400, 200);
}

void Magacin::setupForm()
{
     QGridLayout *mainLayout = new QGridLayout(this);

     m_pbBoi = new QPushButton(BOI, this);
     m_pbBoi->setFixedWidth(PB_FIXEDWIDTH);
     m_pbBoi->setFixedHeight(PB_FIXEDHEIGHT);
     m_pbBoi->setStyleSheet(PB_FONTSIZE);
     m_pbModeli = new QPushButton(MODELI, this);
     m_pbMaterijali = new QPushButton(MATERIJALI, this);

     m_leSearch = new QLineEdit(this);
     m_leSearch->setPlaceholderText(SEARCH_PLACEHOLDER_TEXT);

     mainLayout->addWidget(m_pbBoi, 0, 0);
     mainLayout->addWidget(m_pbModeli, 0, 1);
     mainLayout->addWidget(m_pbMaterijali, 0, 2);
     mainLayout->addWidget(m_leSearch, 1, 0);

}
