#include "Warehouse.h"

#include <QShortcut>
#include <QSqlQueryModel>
#include <QTableView>
#include <QGridLayout>
#include <QLineEdit>
#include <QSqlQuery>

#include "appconsts.h"
#include "HelperFunctions.h"

using namespace Warehouse_NS;
Warehouse::Warehouse(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(WAREHOUSE);
    setupForm();
    setFixedSize(HelperFunctions::desktopWidth() * 0.5, HelperFunctions::desktopHeight() * 0.4);

    connect(m_leSearch, &QLineEdit::returnPressed, this, &Warehouse::slotSearch);
}

void Warehouse::setupForm()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    m_leSearch = new QLineEdit(this);
    HelperFunctions::setWidgetProperties(*m_leSearch);

    m_model = new QSqlQueryModel(this);
    m_table = new QTableView(this);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setModel(m_model);

    mainLayout->addWidget(m_leSearch, 1, 0, 1, 3);
    mainLayout->addWidget(m_table, 2, 0, 1, 3);
}

void Warehouse::slotSearch()
{
    if(m_leSearch->hasFocus())
    {
        //Ako search e prazno selektiraj gi site
        if(m_leSearch->text().trimmed().isEmpty())
        {
            QSqlQuery modelQuery("SELECT sifra, boja, materijal, model, broj, cena, lager FROM obuvki");
            m_model->setQuery(modelQuery);
        }
        //ako ne e prazno filtriraj
        else
        {
            bool madeOfNumbers = false;
            QString searchText = m_leSearch->text();
            for(int i = 0 ; i < searchText.length() ; i++)
            {
                if(searchText[i].isDigit())
                {
                    madeOfNumbers = true;
                    break;
                }
            }
            //dokolku search se brojki togash selektiraj po sifra i broj
            if(madeOfNumbers)
            {
                QString modelQuery(QString("SELECT sifra, boja, materijal, model, broj, cena, lager FROM obuvki"
                                           " WHERE sifra = %1 OR broj=%1").arg(searchText));
                m_model->setQuery(modelQuery);
            }
            //ako e string togash selektiraj spored ostanato
            else
            {
                QString modelQuery(QString("SELECT sifra, boja, materijal, model, broj, cena, lager FROM obuvki"
                                           " WHERE boja ilike '%%1%' OR"
                                           " materijal ilike '%%1%' OR model ilike '%%1%'").arg(searchText));
                m_model->setQuery(modelQuery);
            }
        }
        m_model->setHeaderData(id, Qt::Horizontal, ID);
        m_model->setHeaderData(code, Qt::Horizontal, CODE);
        m_model->setHeaderData(color, Qt::Horizontal, COLOR);
        m_model->setHeaderData(material, Qt::Horizontal, MATERIAL);
        m_model->setHeaderData(model, Qt::Horizontal, MODEL);
        m_model->setHeaderData(shoesize, Qt::Horizontal, SIZE);
        m_model->setHeaderData(price, Qt::Horizontal, PRICE);
        m_model->setHeaderData(stock, Qt::Horizontal, STOCK);

    }
}
