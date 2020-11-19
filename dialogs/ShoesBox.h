#ifndef SHOESBOX_H
#define SHOESBOX_H

#include "GeneralDialog.h"

class QComboBox;
class QPushButton;
class CustomIntLE;

class ShoesBox : public GeneralDialog
{
    Q_OBJECT
public:
    explicit ShoesBox(QWidget *parent = nullptr);

private:
    void setupForm();

    CustomIntLE *m_leCode;

    QComboBox *m_cbColor;
    QComboBox *m_cbModel;
    QComboBox *m_cbMaterial;

    QPushButton *m_pbSave;
    QPushButton *m_pbHide;

    QVector<QWidget*> m_vWidgets;

private slots:
    void newClicked();
    void deleteRecord();
    void filterModel();
    void insertShoe();
    void hideWidgets();
};

#endif // SHOESBOX_H
