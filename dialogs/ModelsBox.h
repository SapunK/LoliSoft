#ifndef MODELS_BOX_H
#define MODELS_BOX_H

#include <GeneralDialog.h>

namespace Models_Box_NS {
enum Columns{
        id,
        model
};
}

class ModelsBox : public GeneralDialog
{
    Q_OBJECT
public:
    explicit ModelsBox(QWidget *parent = nullptr);

private:

private slots:
    void newClicked();
    void deleteRecord();
    void filterModel();

};

#endif // MODELS_BOX_H
