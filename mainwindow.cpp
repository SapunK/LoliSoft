#include "mainwindow.h"
#include "Magacin.h"

//class Magacin;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupForm();
    dbConnect();

    connect(m_pbMagacin,SIGNAL(clicked(bool)),this, SLOT(slotMagacinClicked()));
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

    m_pbMagacin = new QPushButton;
    m_pbMagacin->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbMagacin->setFixedWidth(PB_FIXEDWIDTH);
    m_pbMagacin->setText(MAGACIN);
    m_pbMagacin->setStyleSheet(FONT_WEIGHT_BOLD);
    m_pbMagacin->setStyleSheet(PB_FONTSIZE);

    m_pbProdazba = new QPushButton;
    m_pbProdazba->setFixedHeight(PB_FIXEDHEIGHT);
    m_pbProdazba->setFixedWidth(PB_FIXEDWIDTH);
    m_pbProdazba->setText(PRODAZBA);
    m_pbProdazba->setStyleSheet(FONT_WEIGHT_BOLD);
    m_pbProdazba->setStyleSheet(PB_FONTSIZE);

    //invisible label
    QLabel *invisible = new QLabel;
    invisible->setVisible(false);

    horLayout->addWidget(m_leSearch, 0, Qt::AlignTop);

    grLayout->addLayout(horLayout, 0, 1);

    grLayout->addWidget(m_pbMagacin, 0, 0, 1, 1, Qt::AlignTop);
    grLayout->addWidget(m_pbProdazba, 1, 0, 1, 1, Qt::AlignTop);
    grLayout->addWidget(invisible, 10, 0, 10, 5);

    mainWidget->setLayout(grLayout);

    setCentralWidget(tabWidget);
    centralWidget()->setStyleSheet("background-image: url(\":/other/images/loli_background.jpg\");");

}

void MainWindow::dbConnect()
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
     db.setHostName("127.0.0.1");
     db.setDatabaseName("test");
     db.setUserName("postgres");
     db.setPassword("075692034");
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
    Magacin *mag = new Magacin;
    mag->show();

}

MainWindow::~MainWindow()
{

}
