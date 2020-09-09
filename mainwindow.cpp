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
#include "Colors_Box.h"
#include "appconsts.h"
#include "HelperFunctions.h"
#include "PopulateDatabase.h"
#include "Materials_Box.h"
#include "Models_Box.h"

namespace MainWindow_NS {
#ifdef QT_DEBUG
static const char* CREATE_TABLES = "Create tables";
static const char* FILL_DB = "Fill DB";
#endif

}

using namespace MainWindow_NS;
using namespace Warehouse_NS;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    dbConnect();
    setupForm();

    connect(m_leSearch, &QLineEdit::returnPressed, this, &MainWindow::slotSearch);

    connect(m_pbColors, &QAbstractButton::clicked, this, [this](){
       Colors_Box *box = new Colors_Box(m_mainWidget);
       box->show();
    });
    connect(m_pbWarehouse, &QAbstractButton::clicked, this, [this](){
        Warehouse *box = new Warehouse(this);
        box->show();
    });
    connect(m_pbMaterials, &QAbstractButton::clicked, this, [this](){
        Materials_Box *box = new Materials_Box(this);
        box->show();
    });
    connect(m_pbModels, &QAbstractButton::clicked, this, [this](){
        Models_Box *box = new Models_Box(this);
        box->show();
    });
}

void MainWindow::setupForm()
{
    m_mainWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *vLayoutButtons = new QVBoxLayout;
    QVBoxLayout *vLayoutTable = new QVBoxLayout;

    HelperFunctions::setDesktopSize();

    //    QFile stylesheet(":/style/styles.css");
    //    stylesheet.open(QIODevice::ReadOnly);
    //    QTextStream ts(&stylesheet);

    setWindowState(Qt::WindowMaximized);

    m_leSearch = new QLineEdit(this);
    HelperFunctions::setWidgetProperties(*m_leSearch);

    m_pbWarehouse = new QPushButton(WAREHOUSE, this);
    m_pbSell = new QPushButton(SELL, this);
    m_pbColors = new QPushButton(COLORS, this);
    m_pbModels = new QPushButton(MODELS, this);
    m_pbMaterials = new QPushButton(MATERIALS, this);

    foreach(QPushButton* pb, findChildren<QPushButton*>()){
        HelperFunctions::setWidgetProperties(*pb);
    }

    m_model = new QSqlQueryModel(this);
    //TODO create a custom table class
    m_table = new QTableView(this);
    m_table->setSelectionMode(QTableView::SingleSelection);
    m_table->setSelectionBehavior(QTableView::SelectRows);
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
#ifdef QT_DEBUG
    //For testing purposes
    m_pbCreateTables = new QPushButton(CREATE_TABLES, this);
    m_pbFillDb = new QPushButton(FILL_DB, this);
    vLayoutButtons->addWidget(m_pbCreateTables);
    vLayoutButtons->addWidget(m_pbFillDb);

    connect(m_pbCreateTables, &QPushButton::clicked, &PopulateDatabase::createDatabase);
    connect(m_pbFillDb, &QPushButton::clicked, &PopulateDatabase::fillDatabase);
#endif

    vLayoutTable->addWidget(m_leSearch);
    vLayoutTable->addWidget(m_table);

    mainLayout->addLayout(vLayoutButtons);
    mainLayout->addLayout(vLayoutTable);
    m_mainWidget->setLayout(mainLayout);

    setCentralWidget(m_mainWidget);
    m_mainWidget->show();
//    centralWidget()->setStyleSheet("background-image: url(\":/other/images/loli_background.jpg\");");

}

void MainWindow::slotSearch()
{
    //TODO add stock
    QString modelQuery = "SELECT s.code, c.color, ma.material, mo.model, s.size, s.price "
                         "FROM shoes s "
                         "INNER JOIN colors c on s.color = c.id "
                         "INNER JOIN materials ma on s.material = ma.id "
                         "INNER JOIN models mo on s.model = mo.id ";
    if(!m_leSearch->text().isEmpty()){
        bool madeOfNumbers = false;
        int codeSize = m_leSearch->text().toInt(&madeOfNumbers);

        modelQuery.append(QString("WHERE c.color ilike '%1%' OR "
                                  "ma.material ilike '%1%' OR "
                                  "mo.model ilike '%1%' ").arg(m_leSearch->text()));

        if(madeOfNumbers){
            modelQuery.append(QString("OR s.code = %1 OR s.size = %1").arg(codeSize));
        }
    }
    m_model->setQuery(modelQuery);
    m_table->resizeColumnsToContents();
}

void MainWindow::dbConnect()
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
     db.setHostName("127.0.0.1");
     db.setDatabaseName("shoestore");
     db.setUserName("postgres");
     db.setPassword("postgres");
     db.setPort(5432);

     bool bSuccessfulConn = db.open();

     if(bSuccessfulConn != true)
     {
         qDebug()<<"DB Connect fail";
     }
     else {
         qDebug()<<"Successful DB connection";
     }
}

MainWindow::~MainWindow()
{

}
