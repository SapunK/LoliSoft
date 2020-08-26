#include "Colors_Box.h"

#include "appconsts.h"

namespace Colors_Box_NS{
static const char* SEARCH_COLORS_PLACEHOLDER = "Search by color";

}

using namespace Colors_Box_NS;

Colors_Box::Colors_Box(QDialog *parent) : QDialog(parent)
{
    setWindowTitle(COLORS);
    setupForm();
    connect(m_pbAdd, &QAbstractButton::clicked, this, &Colors_Box::slotDodadiClicked);
    connect(m_pbClose, &QAbstractButton::clicked, this, &Colors_Box::slotZatvoriClicked);
}

void Colors_Box::setupForm()
{
    m_leColor = new QLineEdit(this);
    m_leColor->setPlaceholderText(SEARCH_COLORS_PLACEHOLDER);

    //ova valjda kje bide new button a ne dodadi(insert)
    m_pbAdd = new QPushButton(this);
    m_pbAdd->setText(DODADI);

    m_pbClose = new QPushButton(this);
    m_pbClose->setText(CLOSE);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(m_leColor, 0, 0, 1, 2);
    mainLayout->addWidget(m_pbAdd, 1, 0, 1, 1);
    mainLayout->addWidget(m_pbClose, 1, 1, 1, 1);
}

void Colors_Box::slotDodadiClicked()
{
    if(m_leColor->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Празно поле", "Полето за боја е празно, прво внесете\n"
                                                      " нешто во полето, потоа стиснете додади");
    }
    else
    {
        QString queryString = QString("INSERT INTO boi (boja) VALUES('%1')").arg(m_leColor->text());
        QSqlQuery query;
        if(query.exec(queryString))
        {
            QMessageBox::information(this, "Успешно", "Успешно внесена боја");
        }
        else
        {
            QMessageBox::critical(this, "Грешка", QString("Се случи грешка при внесување\n"
                                                  "'%1'").arg(query.lastQuery()));
        }
    }
}
void Colors_Box::slotZatvoriClicked()
{
    reject();
}
