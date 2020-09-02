#ifndef BOI_BOX_H
#define BOI_BOX_H

#include <QDialog>

class QLineEdit;
class QTableView;
class QSqlQueryModel;

class Colors_Box : public QDialog
{
    Q_OBJECT
public:
    explicit Colors_Box(QWidget *parent = nullptr);

protected:
    void setupForm();

    QLineEdit *m_leColorSearch;
    QLineEdit *m_leNewColor;

    QPushButton *m_pbNew;
    QPushButton *m_pbClose;

    QTableView *m_table;
    QSqlQueryModel *m_model;

public slots:
    void newClicked();

};

#endif // BOI_BOX_H
