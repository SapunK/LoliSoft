#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupForm();
}

void MainWindow::setupForm()
{
    QFile stylesheet(":/other/style/styles.css");
    stylesheet.open(QIODevice::ReadOnly);
    QTextStream ts(&stylesheet);
    setStyleSheet(ts.readAll());
    QGridLayout *grLayout = new QGridLayout;

    qDebug()<<"Error info = "<< stylesheet.errorString();
    QLabel *loliLbl = new QLabel;
    loliLbl->setText(tr("Лоли"));
    loliLbl->setObjectName("test");

    pbTest = new QPushButton;
    pbTest->setText("testiranje");
    pbTest->setObjectName("test");
    QPixmap backgroundLoli;

    pbTest->setStyleSheet("image: url(qrc:/images/images/loli_background.jpg)");

    grLayout->addWidget(loliLbl, 0, 0);
    grLayout->addWidget(pbTest, 0, 1);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(grLayout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{

}
