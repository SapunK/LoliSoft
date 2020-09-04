#ifndef MATERIALS_BOX_H
#define MATERIALS_BOX_H

#include <QDialog>

class QLineEdit;
class QTableView;
class QSqlQueryModel;

namespace Materials_Box_NS {
enum Columns{
        id,
        material
};
}

class Materials_Box : public QDialog
{
    Q_OBJECT
public:
    explicit Materials_Box(QWidget *parent = nullptr);

private:
    void setupForm();

    QLineEdit *m_leMaterialSearch;
    QLineEdit *m_leNewMaterial;

    QPushButton *m_pbNew;
    QPushButton *m_pbClose;
    QPushButton *m_pbDelete;

    QTableView *m_table;
    QSqlQueryModel *m_model;

private slots:
    void newClicked();
    void deleteRecord();

Q_SIGNALS:
    void refreshModel();

};

#endif // MATERIALS_BOX_H
