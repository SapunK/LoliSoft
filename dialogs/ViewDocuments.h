#ifndef VIEWDOCUMENTS_H
#define VIEWDOCUMENTS_H

#include "QDialog"

namespace ViewDocuments_NS{
enum EColumns{
    doc_type_id,
    doc_name,
    doc_number,
    entry_price,
    rebate,
    tax,
    sale_price,
    price_diff,
    datetime
};
}

class QSqlQueryModel;
class CustomTableView;
class QComboBox;

class ViewDocuments : public QDialog
{
Q_OBJECT
public:
    explicit ViewDocuments(QWidget *parent);

private:
    void setupForm();

    QSqlQueryModel *m_model;
    CustomTableView *m_table;
    QComboBox *m_cbDocTypes;
private slots:
    void filterModel();

};

#endif // VIEWDOCUMENTS_H
