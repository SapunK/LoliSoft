#include "ShoesBox.h"

#include <QSqlQueryModel>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

#include "HelperFunctions.h"
#include "custom_widgets/CustomTableView.h"
#include "appconsts.h"
#include "custom_widgets/CustomIntLE.h"

namespace ShoesBox_NS {
static const char* Q_SELECT_SHOES = "SELECT s.id, s.code, m.model, c.color, ma.material "
                                    "FROM shoes s "
                                    "INNER JOIN models m on s.model = m.id "
                                    "INNER JOIN materials ma on s.material = ma.id "
                                    "INNER JOIN colors c on s.color = c.id;";
static const char* Q_SELECT_COLORS = "SELECT color, id FROM colors;";
static const char* Q_SELECT_MODELS = "SELECT model, id FROM models;";
static const char* Q_SELECT_MATERIALS = "SELECT material, id FROM materials;";
static const QString Q_INSERT_SHOE = "INSERT INTO shoes (code, model, color, material) VALUES (%1, %2, %3, %4)";
static const QString Q_DELETE_SHOE = "DELETE FROM shoes WHERE id = %1";
static const char* SAVE = "Save";
static const char* HIDE = "Hide";
}

using namespace ShoesBox_NS;

ShoesBox::ShoesBox(QWidget *parent)
    : GeneralDialog(parent, true)
{
    setWindowTitle(SHOES);
    setFixedSize(HelperFunctions::desktopWidth() * 0.35, HelperFunctions::desktopHeight() * 0.25);
    setupForm();
}

void ShoesBox::setupForm()
{
    m_leCode = new CustomIntLE(this);

    QSqlQueryModel *mColors = new QSqlQueryModel(this);
    mColors->setQuery(Q_SELECT_COLORS);
    m_cbColor = new QComboBox(this);
    m_cbColor->setModel(mColors);

    QSqlQueryModel *mModels = new QSqlQueryModel(this);
    mModels->setQuery(Q_SELECT_MODELS);
    m_cbModel = new QComboBox(this);
    m_cbModel->setModel(mModels);

    QSqlQueryModel *mMaterials = new QSqlQueryModel(this);
    mMaterials->setQuery(Q_SELECT_MATERIALS);
    m_cbMaterial = new QComboBox(this);
    m_cbMaterial->setModel(mMaterials);

    QLabel *lbCode = new QLabel(CODE, this);
    QLabel *lbColor = new QLabel(COLOR, this);
    QLabel *lbModel = new QLabel(MODEL, this);
    QLabel *lbMaterial = new QLabel(MATERIAL, this);

    m_pbSave = new QPushButton(SAVE, this);
    m_pbHide = new QPushButton(HIDE, this);

    //Widgets for show/hide
    m_vWidgets<<m_leCode<<m_cbColor<<m_cbModel<<m_cbMaterial<<m_pbSave
              <<m_pbHide<<lbCode<<lbColor<<lbModel<<lbMaterial;

    for(int i = 0 ; i < m_vWidgets.size() ; i++)
    {
        m_vWidgets.at(i)->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    hideWidgets();

    connect(m_pbNew, &QPushButton::clicked, this, &ShoesBox::hideWidgets);
    connect(m_pbHide, &QPushButton::clicked, this, &ShoesBox::hideWidgets);
    connect(m_pbDelete, &QPushButton::clicked, this, &ShoesBox::deleteRecord);
    connect(m_pbSave, &QPushButton::clicked, this, &ShoesBox::insertShoe);

    m_grLayout->addWidget(lbCode, 0, 0);
    m_grLayout->addWidget(m_leCode, 0, 1);

    m_grLayout->addWidget(lbColor, 1, 0);
    m_grLayout->addWidget(m_cbColor, 1, 1);

    m_grLayout->addWidget(lbModel, 2, 0);
    m_grLayout->addWidget(m_cbModel, 2, 1);

    m_grLayout->addWidget(lbMaterial, 3, 0);
    m_grLayout->addWidget(m_cbMaterial, 3, 1);

    m_grLayout->addWidget(m_pbSave, 4, 0);
    m_grLayout->addWidget(m_pbHide, 4, 1);

    m_grLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 5, 0);

    m_model->setQuery(Q_SELECT_SHOES);
    m_table->resizeColumnsToContents();
    //TODO make an enum and set header data for column names + show only useful columns (we don't need ID)
}

void ShoesBox::newClicked()
{

}

void ShoesBox::deleteRecord()
{
    QSqlQuery q;
    //TODO use enum for column
    int shoeId = m_model->index(m_table->currentIndex().row(), 0).data().toInt();
    if(!q.exec(Q_DELETE_SHOE.arg(shoeId))){
        qCritical()<<"SQL ERROR: "<<q.lastError()<<"\nExecuted query: "<<q.lastQuery();
        QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
    }
    else
        refreshModel();
}

void ShoesBox::filterModel()
{

}

void ShoesBox::insertShoe()
{
    QSqlQuery q;
    int modelId = m_cbModel->model()->index(m_cbModel->currentIndex(), 1).data().toInt();
    int colorId = m_cbColor->model()->index(m_cbColor->currentIndex(), 1).data().toInt();
    int materialId = m_cbMaterial->model()->index(m_cbMaterial->currentIndex(), 1).data().toInt();
    q.prepare(Q_INSERT_SHOE.arg(m_leCode->text()).arg(modelId).arg(colorId).arg(materialId));
    if(q.exec()){
        hideWidgets();
        refreshModel();
    } else {
        qCritical()<<"SQL ERROR: "<<q.lastError()<<"\nExecuted query: "<<q.lastQuery();
        QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
    }

}

void ShoesBox::hideWidgets()
{
    bool hide = true;

    if(sender() == m_pbNew)
        hide = false;

    for(int i = 0 ; i < m_vWidgets.size() ; i++)
    {
        m_vWidgets.at(i)->setHidden(hide);
    }
}
