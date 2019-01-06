#include "Boi_Box.h"
#include "appconsts.h"

Boi_Box::Boi_Box(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("Бои");
    setupForm();
    connect(m_dodadi, SIGNAL(clicked(bool)), this, SLOT(slotDodadiClicked()));
    connect(m_zatvori, SIGNAL(clicked(bool)), this, SLOT(slotZatvoriClicked()));
}

void Boi_Box::setupForm()
{
    m_leBoja = new QLineEdit(this);
    m_leBoja->setPlaceholderText("Внеси боја");

    m_dodadi = new QPushButton(this);
    m_dodadi->setText(DODADI);

    m_zatvori = new QPushButton(this);
    m_zatvori->setText(ZATVORI);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(m_leBoja, 0, 0, 1, 2);
    mainLayout->addWidget(m_dodadi, 1, 0, 1, 1);
    mainLayout->addWidget(m_zatvori, 1, 1, 1, 1);
}

void Boi_Box::slotDodadiClicked()
{
    if(m_leBoja->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Празно поле", "Полето за боја е празно, прво внесете\n"
                                                      " нешто во полето, потоа стиснете додади");
    }
    else
    {
        QString queryString = QString("INSERT INTO boi (boja) VALUES('%1')").arg(m_leBoja->text());
        QSqlQuery query;
        if(query.exec(queryString))
        {
            QMessageBox::information(this, "Успешно", "Успешно внесена боја");
        }
        else
        {
            QMessageBox::critical(this, "Грешка", QString("Се случи грешка при внесување\n"
                                                  "'%1'").arg(query.lastQuery()));
        }
    }
}
void Boi_Box::slotZatvoriClicked()
{
    reject();
}
