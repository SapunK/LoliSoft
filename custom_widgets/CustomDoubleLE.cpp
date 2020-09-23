#include "CustomDoubleLE.h"

#include <QDoubleValidator>

CustomDoubleLE::CustomDoubleLE(QWidget *parent, int bottom, int top, int decimals)
    : QLineEdit(parent)
{
    QDoubleValidator *dValidator = new QDoubleValidator(bottom, top, decimals, this);
    setValidator(dValidator);
    setText("0.00");
}
