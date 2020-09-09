#ifndef MODELS_BOX_H
#define MODELS_BOX_H

#include <QDialog>

class QLineEdit;
class QTableView;
class QSqlQueryModel;

namespace Models_Box_NS {
enum Columns{
        id,
        model
};
}

class Models_Box : public QDialog
{
    Q_OBJECT
public:
    explicit Models_Box(QWidget *parent = nullptr);

private:
    void setupForm();

    QLineEdit *m_leModelSearch;
    QLineEdit *m_leNewModel;

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

#endif // MODELS_BOX_H
