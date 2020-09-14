#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QDialog>

class QLineEdit;
class QTableView;
class QSqlQueryModel;


class GeneralDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GeneralDialog(QWidget *parent = nullptr);

protected:
    void setupForm();

    QLineEdit *m_leSearch;
    QLineEdit *m_leNew;

    QPushButton *m_pbNew;
    QPushButton *m_pbClose;
    QPushButton *m_pbDelete;

    QTableView *m_table;
    QSqlQueryModel *m_model;

protected slots:
    virtual void newClicked() = 0;
    virtual void deleteRecord() = 0;
    virtual void filterModel() = 0;

signals:
    virtual void refreshModel();

};

#endif // GENERALDIALOG_H
