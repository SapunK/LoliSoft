#include "CustomDoubleLE.h"

#include <QDoubleValidator>
#include <QKeyEvent>
#include <QDebug>
#include <QEvent>

CustomDoubleLE::CustomDoubleLE(QWidget *parent, int bottom, int top, int decimals)
    : QLineEdit(parent)
{
    QDoubleValidator *dValidator = new QDoubleValidator(bottom, top, decimals, this);
    setValidator(dValidator);
    setText("0.00");
    connect(this, &QLineEdit::textChanged, this, [this]{
        if(text().isEmpty())
            setText("0.00");
    });
}

double CustomDoubleLE::value()
{
    return text().toDouble();
}

//void CustomDoubleLE::focusOutEvent(QFocusEvent *event)
//{
//    if(text().isEmpty())
//        setText("0.00");
//    QLineEdit::focusOutEvent(event);
//}
