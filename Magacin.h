#ifndef MAGACIN_H
#define MAGACIN_H

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QTableView>
#include <QShortcut>
#include <QDebug>

#include <appconsts.h>

namespace MagacinNS{
enum lastClicked{
    obuvki,
    boi,
    materijali,
    modeli
};
}
class Magacin : public QDialog
{
    Q_OBJECT
public:
    explicit Magacin(QDialog *parent = nullptr);

protected:
    void setupForm();

    int poslednoKliknato;

    QPushButton *m_pbBoi;
    QPushButton *m_pbModeli;
    QPushButton *m_pbMaterijali;

    QPushButton *m_nov;
    QPushButton *m_promeni;
    QPushButton *m_izbrishi;

    QLineEdit *m_leSearch;

    QSqlQueryModel *m_model;
    QTableView *m_table;
signals:

public slots:
    void slotBoiClicked();
    void slotModeliClicked();
    void slotMaterijaliClicked();
    void slotSearchLE();
    void slotNovClicked();
};

#endif // MAGACIN_H
