#include "mainwindow.h"

#include <QGridLayout>
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
    QTabWidget *tabWidget = new QTabWidget;
    QWidget *mainWidget = new QWidget;
    QGridLayout *grLayout = new QGridLayout;
    QHBoxLayout *horLayout = new QHBoxLayout;

    tabWidget->addTab(mainWidget, POCETNA);
    tabWidget->setTabsClosable(true);

    //    QFile stylesheet(":/style/styles.css");
    //    stylesheet.open(QIODevice::ReadOnly);
    //    QTextStream ts(&stylesheet);
    //    mainWidget->setStyleSheet(ts.readAll());

    setWindowState(Qt::WindowMaximized);

    m_leSearch = new QLineEdit;
    m_leSearch->setPlaceholderText(SEARCH_PLACEHOLDER_TEXT);
    m_leSearch->setStyleSheet(SEARCHLE_FONTSIZE);
    m_leSearch->setFixedWidth(SEARCHLE_FIXEDWIDTH);
    m_leSearch->setFixedHeight(SEARCHLE_FIXEDHEIGHT);

    m_pbWarehouse = new QPushButton;
    m_pbWarehouse->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbWarehouse->setFixedWidth(PB_FIXEDWIDTH);
    m_pbWarehouse->setText(MAGACIN);
    m_pbWarehouse->setStyleSheet(FONT_WEIGHT_BOLD);
    m_pbWarehouse->setStyleSheet(PB_FONTSIZE);

    m_pbSell = new QPushButton;
    m_pbSell->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbSell->setFixedWidth(PB_FIXEDWIDTH);
    m_pbSell->setText(PRODAZBA);
    m_pbSell->setStyleSheet(FONT_WEIGHT_BOLD);
    m_pbSell->setStyleSheet(PB_FONTSIZE);

    m_model = new QSqlQueryModel;
    m_table = new QTableView;
    m_table->setModel(m_model);

    m_model->setHeaderData(0, Qt::Horizontal, "Шифра");
    m_model->setHeaderData(1, Qt::Horizontal, "Боја");
    m_model->setHeaderData(2, Qt::Horizontal, "Материјал");
    m_model->setHeaderData(3, Qt::Horizontal, "Модел");
    m_model->setHeaderData(4, Qt::Horizontal, "Број");
    m_model->setHeaderData(5, Qt::Horizontal, "Цена");
    m_model->setHeaderData(6, Qt::Horizontal, "Лагер");

    //invisible label
    QLabel *invisible = new QLabel;
    invisible->setVisible(false);

    horLayout->addWidget(m_leSearch, 0, Qt::AlignTop);

    grLayout->addLayout(horLayout, 0, 1);

    grLayout->addWidget(m_pbWarehouse, 0, 0, 1, 1, Qt::AlignTop);
    grLayout->addWidget(m_pbSell, 1, 0, 1, 1, Qt::AlignTop);
    grLayout->addWidget(m_table, 2, 1);
    grLayout->addWidget(invisible, 10, 0, 10, 5);

    mainWidget->setLayout(grLayout);

//    setCentralWidget(tabWidget);
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
    }
}

void MainWindow::dbConnect()
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
     db.setHostName("127.0.0.1");
     db.setDatabaseName("shoestore");
     db.setUserName("postgres");
     db.setPassword("postgres");
     db.setPort(5432);

     bool oks = db.open();

     if(oks != true)
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
