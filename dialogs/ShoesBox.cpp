#include "ShoesBox.h"

#include <QSqlQueryModel>
#include <QGridLayout>
#include <QPushButton>

#include "appconsts.h"

namespace ShoesBox_NS {
//TODO select only relevant columns
    static const char* Q_SELECT_SHOES = "SELECT * FROM shoes;";
}

using namespace ShoesBox_NS;

ShoesBox::ShoesBox(QWidget *parent)
    : GeneralDialog(parent, true)
{
    setWindowTitle(SHOES);

    m_model->setQuery(Q_SELECT_SHOES);

    //TODO add some kind of form to add a new shoe (probably in m_grLayout)
}

void ShoesBox::newClicked()
{

}

void ShoesBox::deleteRecord()
{

}

void ShoesBox::filterModel()
{

}
