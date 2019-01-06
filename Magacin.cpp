#include "Magacin.h"
#include "Boi_Box.h"

using namespace MagacinNS;
Magacin::Magacin(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("Магацин");
    setupForm();
    setFixedSize(1000, 600);
    poslednoKliknato = obuvki;

    connect(m_pbBoi, SIGNAL(clicked(bool)), this, SLOT(slotBoiClicked()));
    connect(m_pbModeli, SIGNAL(clicked(bool)), this, SLOT(slotModeliClicked()));
    connect(m_pbMaterijali, SIGNAL(clicked(bool)), this, SLOT(slotMaterijaliClicked()));
    QShortcut *searchShortcut = new QShortcut(Qt::Key_Return, this);
    connect(searchShortcut, SIGNAL(activated()), this, SLOT(slotSearchLE()));
}

void Magacin::setupForm()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    m_pbBoi = new QPushButton(BOI, this);
    m_pbBoi->setFixedWidth(PB_FIXEDWIDTH);
    m_pbBoi->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbBoi->setStyleSheet(PB_FONTSIZE);

    m_pbModeli = new QPushButton(MODELI, this);
    m_pbModeli->setFixedWidth(PB_FIXEDWIDTH);
    m_pbModeli->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbModeli->setStyleSheet(PB_FONTSIZE);

    m_pbMaterijali = new QPushButton(MATERIJALI, this);
    m_pbMaterijali->setFixedWidth(PB_FIXEDWIDTH);
    m_pbMaterijali->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbMaterijali->setStyleSheet(PB_FONTSIZE);

    m_leSearch = new QLineEdit(this);
    m_leSearch->setPlaceholderText(SEARCH_PLACEHOLDER_TEXT);
    m_leSearch->setStyleSheet(SEARCHLE_FONTSIZE);
    m_leSearch->setFixedHeight(SEARCHLE_FIXEDHEIGHT);

    m_model = new QSqlQueryModel(this);
    m_table = new QTableView(this);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setModel(m_model);

    m_nov = new QPushButton(NOV);
    m_nov->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_nov->setHidden(true);

    m_promeni = new QPushButton(PROMENI);
    m_promeni->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_promeni->setHidden(true);

    m_izbrishi = new QPushButton(IZBRISI);
    m_izbrishi->setFixedWidth(SMALLPB_FIXEDWIDTH);
    m_izbrishi->setHidden(true);

    buttonsLayout->addWidget(m_nov);
    buttonsLayout->addWidget(m_promeni);
    buttonsLayout->addWidget(m_izbrishi);

    mainLayout->addWidget(m_pbBoi, 0, 0);
    mainLayout->addWidget(m_pbModeli, 0, 1);
    mainLayout->addWidget(m_pbMaterijali, 0, 2);
    mainLayout->addWidget(m_leSearch, 1, 0, 1, 3);
    mainLayout->addLayout(buttonsLayout, 2, 0);
    mainLayout->addWidget(m_table, 3, 0, 1, 3);
}

void Magacin::slotBoiClicked()
{
    poslednoKliknato = boi;
    QSqlQuery modelQuery("SELECT sifra, boja FROM boi");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Боја");

    m_nov->setText(tr("Нова"));
    m_nov->setHidden(false);
    m_promeni->setHidden(false);
    m_izbrishi->setHidden(false);
}

void Magacin::slotModeliClicked()
{
    poslednoKliknato = modeli;
    QSqlQuery modelQuery("SELECT sifra, model FROM modeli");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Модел");

    m_nov->setText(NOV);
    m_nov->setHidden(false);
    m_promeni->setHidden(false);
    m_izbrishi->setHidden(false);
}

void Magacin::slotMaterijaliClicked()
{
    poslednoKliknato = materijali;
    QSqlQuery modelQuery("SELECT sifra, materijal FROM materijali");
    m_model->setQuery(modelQuery);
    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Материјал");

    m_nov->setText(NOV);
    m_nov->setHidden(false);
    m_promeni->setHidden(false);
    m_izbrishi->setHidden(false);
}

void Magacin::slotSearchLE()
{
    if(m_leSearch->hasFocus())
    {
        m_nov->setText("Нова");
        m_nov->setHidden(false);
        m_promeni->setHidden(false);
        m_izbrishi->setHidden(false);
        poslednoKliknato = obuvki;
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

void Magacin::slotNovClicked()
{

}
