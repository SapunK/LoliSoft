#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupForm();
}

void MainWindow::setupForm()
{
    QGridLayout *grLayout = new QGridLayout;

    QLabel *loliLbl = new QLabel;
    loliLbl->setText(tr("Лоли"));

    pbTest = new QPushButton;

    grLayout->addWidget(loliLbl, 0, 0);
    grLayout->addWidget(pbTest, 0, 1);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(grLayout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{

}
