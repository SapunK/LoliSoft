#include "CustomIntLE.h"

#include <QIntValidator>

CustomIntLE::CustomIntLE(QWidget *parent, int bottom, int top)
    : QLineEdit(parent)
{
    QIntValidator *iValidator = new QIntValidator(bottom, top, this);
    setValidator(iValidator);
    setText("0");
}
