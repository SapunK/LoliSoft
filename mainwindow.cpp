#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupForm();
    dbConnect();
}

void MainWindow::setupForm()
{
    QTabWidget *tabWidget = new QTabWidget;
    QWidget *mainWidget = new QWidget;
    QGridLayout *grLayout = new QGridLayout;
    QHBoxLayout *horLayout = new QHBoxLayout;

    tabWidget->addTab(mainWidget, "Glaven");
    tabWidget->setTabsClosable(true);

    //    QFile stylesheet(":/style/styles.css");
    //    stylesheet.open(QIODevice::ReadOnly);
//    QTextStream ts(&stylesheet);
//    mainWidget->setStyleSheet(ts.readAll());

    setWindowState(Qt::WindowMaximized);

    m_leSearch = new QLineEdit;
    m_leSearch->setPlaceholderText(PLACEHOLDER_TEXT);
    m_leSearch->setStyleSheet("font-size:23px;");
    m_leSearch->setFixedWidth(500);
    m_leSearch->setFixedHeight(40);

    m_pbMagacin = new QPushButton;
    m_pbMagacin->setFixedHeight(40);
    m_pbMagacin->setFixedWidth(250);
    m_pbMagacin->setText(MAGACIN);
    m_pbMagacin->setStyleSheet("font-weight:bold;");
    m_pbMagacin->setStyleSheet("font-size:23px;");

    m_pbProdazba = new QPushButton;
    m_pbProdazba->setFixedHeight(40);
    m_pbProdazba->setFixedWidth(250);
    m_pbProdazba->setText(PRODAZBA);
    m_pbProdazba->setStyleSheet("font-weight:bold;");
    m_pbProdazba->setStyleSheet("font-size:23px;");

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
     db.driver()->open("QPSQL");
     qDebug()<<"db driver isopen= "<<db.driver()->isOpen();
    // qDebug()<<"Db error = "<<
     bool oks = db.open();

     if(oks != true)
     {
         qDebug()<<"DB Connect fail";
     }
     else {
         qDebug()<<"DB connect good";
     }
}

MainWindow::~MainWindow()
{

}
