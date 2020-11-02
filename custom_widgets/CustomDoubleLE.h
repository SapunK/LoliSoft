#ifndef CUSTOMDOUBLELE_H
#define CUSTOMDOUBLELE_H


#include <QLineEdit>

class CustomDoubleLE : public QLineEdit
{
public:
    explicit CustomDoubleLE(QWidget *parent = nullptr, int bottom = 0, int top = 1e8, int decimals = 2);
    double value();
private:
//    void focusOutEvent(QFocusEvent *event);
};

#endif // CUSTOMDOUBLELE_H
