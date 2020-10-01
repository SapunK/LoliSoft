#include "CustomIntLE.h"

#include <QIntValidator>

CustomIntLE::CustomIntLE(QWidget *parent, int bottom, int top)
    : QLineEdit(parent)
{
    QIntValidator *iValidator = new QIntValidator(bottom, top, this);
    setValidator(iValidator);
    setText("0");

}

int CustomIntLE::value()
{
   return text().toInt();
}

void CustomIntLE::focusOutEvent(QFocusEvent *event)
{
    if(text().isEmpty())
        setText("0");
    QLineEdit::focusOutEvent(event);
}
