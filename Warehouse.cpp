#include "Warehouse.h"

#include <QShortcut>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QTableView>
#include <QGridLayout>
#include <QLineEdit>
#include <QSqlQuery>

#include "Colors_Box.h"
#include "appconsts.h"

using namespace Warehouse_NS;
Warehouse::Warehouse(QDialog *parent) : QDialog(parent)
{
    setWindowTitle(WAREHOUSE);
    setupForm();
    //TODO use screenGeometry to set the size
    setFixedSize(1000, 600);
    lastClicked = shoes;

    connect(m_pbColors, &QAbstractButton::clicked, this, &Warehouse::slotColorsClicked);
    connect(m_pbModels, &QAbstractButton::clicked, this, &Warehouse::slotModelsClicked);
    connect(m_pbMaterials, &QAbstractButton::clicked, this, &Warehouse::slotMaterialsClicked);
    QShortcut *searchShortcut = new QShortcut(Qt::Key_Return, this);
    connect(searchShortcut, &QShortcut::activated, this, &Warehouse::slotSearch);
    connect(m_pbNew, &QAbstractButton::clicked, this, &Warehouse::slotNewClicked);
}

void Warehouse::setupForm()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    m_pbColors = new QPushButton(COLORS, this);
    m_pbColors->setFixedWidth(PB_FIXEDWIDTH);
    m_pbColors->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbColors->setStyleSheet(PB_FONTSIZE);

    m_pbModels = new QPushButton(MODELS, this);
    m_pbModels->setFixedWidth(PB_FIXEDWIDTH);
    m_pbModels->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbModels->setStyleSheet(PB_FONTSIZE);

    m_pbMaterials = new QPushButton(MATERIALS, this);
    m_pbMaterials->setFixedWidth(PB_FIXEDWIDTH);
    m_pbMaterials->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbMaterials->setStyleSheet(PB_FONTSIZE);

    m_leSearch = new QLineEdit(this);
    m_leSearch->setPlaceholderText(SEARCH_PLACEHOLDER_TEXT);
    m_leSearch->setStyleSheet(SEARCHLE_FONTSIZE);
    m_leSearch->setFixedHeight(SEARCHLE_FIXEDHEIGHT);

    m_model = new QSqlQueryModel(this);
    m_table = new QTableView(this);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setModel(m_model);

    m_pbNew = new QPushButton(NEW);
    m_pbNew->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_pbNew->setDisabled(true);

    m_pbChange = new QPushButton(EDIT);
    m_pbChange->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_pbChange->setDisabled(true);

    m_pbDelete = new QPushButton(DELETE);
    m_pbDelete->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_pbDelete->setDisabled(true);

    buttonsLayout->addWidget(m_pbNew);
    buttonsLayout->addWidget(m_pbChange);
    buttonsLayout->addWidget(m_pbDelete);

    mainLayout->addWidget(m_pbColors, 0, 0);
    mainLayout->addWidget(m_pbModels, 0, 1);
    mainLayout->addWidget(m_pbMaterials, 0, 2);
    mainLayout->addWidget(m_leSearch, 1, 0, 1, 3);
    mainLayout->addLayout(buttonsLayout, 2, 0);
    mainLayout->addWidget(m_table, 3, 0, 1, 3);
}

void Warehouse::slotColorsClicked()
{
    lastClicked = colors;
    QSqlQuery modelQuery("SELECT sifra, boja FROM boi");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, ID);
    m_model->setHeaderData(1, Qt::Horizontal, COLOR);

    m_pbNew->setText(NEW);
    m_pbNew->setDisabled(false);
    m_pbChange->setDisabled(false);
    m_pbDelete->setDisabled(false);
}

void Warehouse::slotModelsClicked()
{
    lastClicked = models;
    QSqlQuery modelQuery("SELECT sifra, model FROM modeli");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, ID);
    m_model->setHeaderData(1, Qt::Horizontal, MODEL);

    m_pbNew->setText(NEW);
    m_pbNew->setDisabled(false);
    m_pbChange->setDisabled(false);
    m_pbDelete->setDisabled(false);
}

void Warehouse::slotMaterialsClicked()
{
    lastClicked = materials;
    QSqlQuery modelQuery("SELECT sifra, materijal FROM materijali");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, ID);
    m_model->setHeaderData(1, Qt::Horizontal, MODEL);

    m_pbNew->setText(NEW);
    m_pbNew->setDisabled(false);
    m_pbChange->setDisabled(false);
    m_pbDelete->setDisabled(false);;
}

void Warehouse::slotSearch()
{
    if(m_leSearch->hasFocus())
    {
        m_pbNew->setText(NEW);
        m_pbNew->setDisabled(false);
        m_pbChange->setDisabled(false);
        m_pbDelete->setDisabled(false);
        lastClicked = shoes;
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

void Warehouse::slotNewClicked()
{
    switch(lastClicked)
    {
    case shoes:
    break;

    case colors:
    Colors_Box *boiB = new Colors_Box;
    boiB->show();
    break;

//    case materijali:
//    break;

//    case modeli:
//    break;

    }
}
