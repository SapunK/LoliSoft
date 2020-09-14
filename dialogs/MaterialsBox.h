#ifndef MATERIALS_BOX_H
#define MATERIALS_BOX_H

#include "GeneralDialog.h"

namespace Materials_Box_NS {
enum Columns{
        id,
        material
};
}

class MaterialsBox : public GeneralDialog
{
    Q_OBJECT
public:
    explicit MaterialsBox(QWidget *parent = nullptr);

private:

private slots:
    void newClicked();
    void deleteRecord();
    void filterModel();

};

#endif // MATERIALS_BOX_H
