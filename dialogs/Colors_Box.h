#ifndef BOI_BOX_H
#define BOI_BOX_H

#include <QDialog>

class QLineEdit;
class QTableView;
class QSqlQueryModel;

namespace Colors_Box_NS {
enum Columns{
        id,
        color
};
}

class Colors_Box : public QDialog
{
    Q_OBJECT
public:
    explicit Colors_Box(QWidget *parent = nullptr);

private:
    void setupForm();

    QLineEdit *m_leColorSearch;
    QLineEdit *m_leNewColor;

    QPushButton *m_pbNew;
    QPushButton *m_pbClose;
    QPushButton *m_pbDelete;

    QTableView *m_table;
    QSqlQueryModel *m_model;

private slots:
    void newClicked();
    void deleteRecord();

signals:
    void refreshModel();

};

#endif // BOI_BOX_H
