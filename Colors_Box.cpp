#include "Colors_Box.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

#include "appconsts.h"

namespace Colors_Box_NS{
static const char* SEARCH_COLORS_PLACEHOLDER = "Search by color";

}

using namespace Colors_Box_NS;

Colors_Box::Colors_Box(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(COLORS);
    setupForm();
    connect(m_pbNew, &QAbstractButton::clicked, this, &Colors_Box::newClicked);
    connect(m_pbClose, &QAbstractButton::clicked, this, &Colors_Box::closeClicked);
}

void Colors_Box::setupForm()
{
    m_leColor = new QLineEdit(this);
    m_leColor->setPlaceholderText(SEARCH_COLORS_PLACEHOLDER);

    m_pbNew = new QPushButton(NEW, this);
    m_pbClose = new QPushButton(CLOSE, this);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(m_leColor, 0, 0, 1, 2);
    mainLayout->addWidget(m_pbNew, 1, 0, 1, 1);
    mainLayout->addWidget(m_pbClose, 1, 1, 1, 1);
}

void Colors_Box::newClicked()
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
void Colors_Box::closeClicked()
{
    reject();
}
