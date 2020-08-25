#ifndef MAGACIN_H
#define MAGACIN_H

#include <QDialog>

class QLineEdit;
class QSqlQueryModel;
class QTableView;

namespace Warehouse_NS{
enum lastClicked{
    shoes,
    colors,
    materials,
    models
};

enum columns{

};
}

class Warehouse : public QDialog
{
    Q_OBJECT
public:
    explicit Warehouse(QDialog *parent = nullptr);

protected:
    void setupForm();

    int lastClicked;

    QPushButton *m_pbColors;
    QPushButton *m_pbModels;
    QPushButton *m_pbMaterials;
    QPushButton *m_pbNew;
    QPushButton *m_pbChange;
    QPushButton *m_pbDelete;

    QLineEdit *m_leSearch;

    QSqlQueryModel *m_model;
    QTableView *m_table;
signals:

public slots:
    void slotColorsClicked();
    void slotModelsClicked();
    void slotMaterialsClicked();
    void slotSearch();
    void slotNewClicked();
};

#endif // MAGACIN_H
