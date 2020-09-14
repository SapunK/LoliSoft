#ifndef COLORS_BOX_H
#define COLORS_BOX_H

#include "GeneralDialog.h"

namespace Colors_Box_NS {
enum Columns{
        id,
        color
};
}

class ColorsBox : public GeneralDialog
{
    Q_OBJECT
public:
    explicit ColorsBox(QWidget *parent = nullptr);

private:

private slots:
    void newClicked();
    void deleteRecord();
    void filterModel();

signals:

};

#endif // COLORS_BOX_H
