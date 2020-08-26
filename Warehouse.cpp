#include "Warehouse.h"

#include <QShortcut>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QTableView>

#include "Colors_Box.h"
#include "appconsts.h"

using namespace Warehouse_NS;
Warehouse::Warehouse(QDialog *parent) : QDialog(parent)
{
    setWindowTitle(WAREHOUSE);
    setupForm();
    setFixedSize(1000, 600);
    lastClicked = shoes;

    connect(m_pbColors, SIGNAL(clicked(bool)), this, SLOT(slotColorsClicked()));
    connect(m_pbModels, SIGNAL(clicked(bool)), this, SLOT(slotModelsClicked()));
    connect(m_pbMaterials, SIGNAL(clicked(bool)), this, SLOT(slotMaterialsClicked()));
    QShortcut *searchShortcut = new QShortcut(Qt::Key_Return, this);
    connect(searchShortcut, SIGNAL(activated()), this, SLOT(slotSearch()));
    connect(m_pbNew, SIGNAL(clicked(bool)), this, SLOT(slotNewClicked()));
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
    m_pbNew->setHidden(true);

    m_pbChange = new QPushButton(EDIT);
    m_pbChange->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_pbChange->setHidden(true);

    m_pbDelete = new QPushButton(DELETE);
    m_pbDelete->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_pbDelete->setHidden(true);

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
    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Боја");

    m_pbNew->setText(tr("Нова"));
    m_pbNew->setHidden(false);
    m_pbChange->setHidden(false);
    m_pbDelete->setHidden(false);
}

void Warehouse::slotModelsClicked()
{
    lastClicked = models;
    QSqlQuery modelQuery("SELECT sifra, model FROM modeli");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Модел");

    m_pbNew->setText(NEW);
    m_pbNew->setHidden(false);
    m_pbChange->setHidden(false);
    m_pbDelete->setHidden(false);
}

void Warehouse::slotMaterialsClicked()
{
    lastClicked = materials;
    QSqlQuery modelQuery("SELECT sifra, materijal FROM materijali");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Материјал");

    m_pbNew->setText(NEW);
    m_pbNew->setHidden(false);
    m_pbChange->setHidden(false);
    m_pbDelete->setHidden(false);
}

void Warehouse::slotSearch()
{
    if(m_leSearch->hasFocus())
    {
        m_pbNew->setText("Нова");
        m_pbNew->setHidden(false);
        m_pbChange->setHidden(false);
        m_pbDelete->setHidden(false);
        lastClicked = shoes;
        //Ako search e prazno selektiraj gi site
        if(m_leSearch->text().isEmpty())
        {
            QSqlQuery modelQuery("SELECT sifra, boja, materijal, model, broj, cena, lager FROM obuvki");
            m_model->setQuery(modelQuery);
        }
        //ako ne e prazno filtriraj
        else
        {
            bool madeOfNumbers;
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
        m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
        m_model->setHeaderData(1, Qt::Horizontal, "Боја");
        m_model->setHeaderData(2, Qt::Horizontal, "Материјал");
        m_model->setHeaderData(3, Qt::Horizontal, "Модел");
        m_model->setHeaderData(4, Qt::Horizontal, "Број");
        m_model->setHeaderData(5, Qt::Horizontal, "Цена");
        m_model->setHeaderData(6, Qt::Horizontal, "Лагер");

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
