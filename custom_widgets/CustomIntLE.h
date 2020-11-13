#ifndef CUSTOMINTLE_H
#define CUSTOMINTLE_H

#include <QLineEdit>

class CustomIntLE : public QLineEdit
{
public:
    explicit CustomIntLE(QWidget *parent = nullptr, int bottom = 0, int top = 1e8);
    int value();
    void setValue(int value);
private:
//    void focusOutEvent(QFocusEvent *event);
};

#endif // CUSTOMINTLE_H
