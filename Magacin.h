#ifndef MAGACIN_H
#define MAGACIN_H

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>

#include <appconsts.h>

class Magacin : public QDialog
{
    Q_OBJECT
public:
    explicit Magacin(QDialog *parent = nullptr);

protected:
    void setupForm();
    QPushButton *m_pbBoi;
    QPushButton *m_pbModeli;
    QPushButton *m_pbMaterijali;

    QLineEdit *m_leSearch;
signals:

public slots:
};

#endif // MAGACIN_H
