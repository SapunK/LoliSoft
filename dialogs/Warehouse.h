#ifndef MAGACIN_H
#define MAGACIN_H

#include <QDialog>

class QLineEdit;
class QSqlQueryModel;
class QTableView;

namespace Warehouse_NS{
enum columns{
    id,
    code,
    color,
    material,
    model,
    shoesize,
    price,
    stock
};
}

class Warehouse : public QDialog
{
    Q_OBJECT
public:
    explicit Warehouse(QWidget *parent = nullptr);

protected:
    void setupForm();

    QPushButton *m_pbColors;
    QPushButton *m_pbModels;
    QPushButton *m_pbMaterials;

    QLineEdit *m_leSearch;

    QSqlQueryModel *m_model;
    QTableView *m_table;
signals:

public slots:
    void slotSearch();
};

#endif // MAGACIN_H
