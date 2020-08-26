#include "mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QDebug>
#include <QShortcut>

#include "Warehouse.h"
#include "appconsts.h"

namespace MainWindow_NS {

}

using namespace MainWindow_NS;
using namespace Warehouse_NS;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    dbConnect();
    setupForm();

    connect(m_pbWarehouse,&QAbstractButton::clicked,this, &MainWindow::slotMagacinClicked);
    QShortcut *searchShortcut = new QShortcut(Qt::Key_Return, this);
    connect(searchShortcut, &QShortcut::activated, this, &MainWindow::slotSearch);
}

void MainWindow::setupForm()
{
//    QTabWidget *tabWidget = new QTabWidget;
    QWidget *mainWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *vLayoutButtons = new QVBoxLayout;
    QVBoxLayout *vLayoutTable = new QVBoxLayout;

//    tabWidget->addTab(mainWidget, POCETNA);
//    tabWidget->setTabsClosable(true);

    //    QFile stylesheet(":/style/styles.css");
    //    stylesheet.open(QIODevice::ReadOnly);
    //    QTextStream ts(&stylesheet);
    //    mainWidget->setStyleSheet(ts.readAll());

    setWindowState(Qt::WindowMaximized);

    m_leSearch = new QLineEdit(this);
    m_leSearch->setPlaceholderText(SEARCH_PLACEHOLDER_TEXT);
    m_leSearch->setStyleSheet(SEARCHLE_FONTSIZE);
    m_leSearch->setFixedWidth(SEARCHLE_FIXEDWIDTH);
    m_leSearch->setFixedHeight(SEARCHLE_FIXEDHEIGHT);

    m_pbWarehouse = new QPushButton(WAREHOUSE, this);
    m_pbSell = new QPushButton(SELL, this);
    m_pbColors = new QPushButton(COLORS, this);
    m_pbModels = new QPushButton(MODELS, this);
    m_pbMaterials = new QPushButton(MATERIALS, this);

    foreach(QPushButton* pb, findChildren<QPushButton*>())
    {
        setPbProperties(*pb);
    }

    m_model = new QSqlQueryModel;
    m_table = new QTableView;
    m_table->setModel(m_model);

    //TODO create a new model and use it here and in warehouse
    m_model->setHeaderData(id, Qt::Horizontal, ID);
    m_model->setHeaderData(code, Qt::Horizontal, CODE);
    m_model->setHeaderData(color, Qt::Horizontal, COLOR);
    m_model->setHeaderData(material, Qt::Horizontal, MATERIAL);
    m_model->setHeaderData(model, Qt::Horizontal, MODEL);
    m_model->setHeaderData(shoesize, Qt::Horizontal, SIZE);
    m_model->setHeaderData(price, Qt::Horizontal, PRICE);
    m_model->setHeaderData(stock, Qt::Horizontal, STOCK);

    vLayoutButtons->addWidget(m_pbWarehouse);
    vLayoutButtons->addWidget(m_pbSell);
    vLayoutButtons->addWidget(m_pbColors);
    vLayoutButtons->addWidget(m_pbMaterials);
    vLayoutButtons->addWidget(m_pbModels);
    vLayoutButtons->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    vLayoutTable->addWidget(m_leSearch);
    vLayoutTable->addWidget(m_table);

    mainLayout->addLayout(vLayoutButtons);
    mainLayout->addLayout(vLayoutTable);
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
    mainWidget->show();
//    centralWidget()->setStyleSheet("background-image: url(\":/other/images/loli_background.jpg\");");

}

void MainWindow::slotSearch()
{
    if(m_leSearch->hasFocus())
    {
        //Ako search e prazno selektiraj gi site
        if(m_leSearch->text().isEmpty())
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
    }
}

void MainWindow::setPbProperties(QPushButton &pb)
{
    pb.setFixedHeight(PB_FIXEDHEIGHT);
    pb.setFixedWidth(PB_FIXEDWIDTH);
    pb.setStyleSheet(FONT_WEIGHT_BOLD);
    pb.setStyleSheet(PB_FONTSIZE);
}

void MainWindow::dbConnect()
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
     db.setHostName("127.0.0.1");
     db.setDatabaseName("shoestore");
     db.setUserName("postgres");
     db.setPassword("postgres");
     db.setPort(5432);

     bool bSucsessfulConn = db.open();

     if(bSucsessfulConn != true)
     {
         qDebug()<<"DB Connect fail";
     }
     else {
         qDebug()<<"DB connect good";
     }
}

void MainWindow::slotMagacinClicked()
{
    Warehouse *mag = new Warehouse;
    mag->show();
}

MainWindow::~MainWindow()
{

}
