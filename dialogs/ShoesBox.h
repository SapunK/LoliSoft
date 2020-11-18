#ifndef SHOESBOX_H
#define SHOESBOX_H

#include "GeneralDialog.h"

class ShoesBox : public GeneralDialog
{
    Q_OBJECT
public:
    explicit ShoesBox(QWidget *parent = nullptr);

private:

private slots:
    void newClicked();
    void deleteRecord();
    void filterModel();
};

#endif // SHOESBOX_H
