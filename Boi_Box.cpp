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

}

void Boi_Box::slotZatvoriClicked()
{

}
