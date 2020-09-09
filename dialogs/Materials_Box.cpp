#include "Materials_Box.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QTableView>
#include <QDebug>

#include "appconsts.h"

namespace Materials_Box_NS{
static const char* SEARCH_MATERIALS_PLACEHOLDER = "Search by material";
static const char* INSERT_MATERIAL = "Insert new material";
static const char* MATERIAL_FIELD_EMPTY = "Material field is empty.\nInsert a new material then press the button.";
static const QString MATERIAL_SEARCH_QUERY = "SELECT id, material FROM materials WHERE material ilike '%%1%';";
}

using namespace Materials_Box_NS;

Materials_Box::Materials_Box(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(MATERIALS);
    setupForm();

    connect(m_pbNew, &QAbstractButton::clicked, this, &Materials_Box::newClicked);
    connect(m_pbClose, &QAbstractButton::clicked, this, &QDialog::close);
    connect(m_leMaterialSearch, &QLineEdit::returnPressed, this, [this](){
        m_model->setQuery(MATERIAL_SEARCH_QUERY.arg(m_leMaterialSearch->text()));
    });
    connect(m_pbDelete, &QPushButton::clicked, this, &Materials_Box::deleteRecord);
    connect(this, &Materials_Box::refreshModel, this, [this]{
       m_model->setQuery(m_model->query().lastQuery());
       m_table->resizeColumnsToContents();
    });
}

void Materials_Box::setupForm()
{
    m_model = new QSqlQueryModel(this);
    m_model->setHeaderData(id, Qt::Horizontal, ID);
    m_model->setHeaderData(material, Qt::Horizontal, MATERIAL);
    m_model->setQuery(MATERIAL_SEARCH_QUERY.arg(""));

    m_table = new QTableView(this);
    m_table->setSelectionMode(QTableView::SingleSelection);
    m_table->setSelectionBehavior(QTableView::SelectRows);
    m_table->setModel(m_model);
    m_table->resizeColumnsToContents();
    m_table->hideColumn(id);

    m_leMaterialSearch = new QLineEdit(this);
    m_leMaterialSearch->setPlaceholderText(SEARCH_MATERIALS_PLACEHOLDER);
    setFocusProxy(m_leMaterialSearch);
    m_leMaterialSearch->setFocus();

    m_leNewMaterial = new QLineEdit(this);
    m_leNewMaterial->setPlaceholderText(INSERT_MATERIAL);

    m_pbNew = new QPushButton(NEW, this);
    m_pbNew->setFocusPolicy(Qt::NoFocus);

    m_pbClose = new QPushButton(CLOSE, this);
    m_pbClose->setFocusPolicy(Qt::NoFocus);

    m_pbDelete = new QPushButton(DELETE, this);
    m_pbDelete->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *vLeftSide = new QVBoxLayout;
    QVBoxLayout *vRightSide = new QVBoxLayout;

    vLeftSide->addWidget(m_leNewMaterial);
    vLeftSide->addWidget(m_pbNew);
    vLeftSide->addWidget(m_pbClose);
    vLeftSide->addWidget(m_pbDelete);
    vLeftSide->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    vRightSide->addWidget(m_leMaterialSearch);
    vRightSide->addWidget(m_table);

    mainLayout->addLayout(vLeftSide);
    mainLayout->addLayout(vRightSide);

    setLayout(mainLayout);
}

void Materials_Box::newClicked()
{
    if(m_leNewMaterial->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, EMPTY_FIELD, MATERIAL_FIELD_EMPTY);
    }
    else
    {
        QString queryString = QString("INSERT INTO materials (material) VALUES('%1')").arg(m_leNewMaterial->text());
        QSqlQuery query;
        if(!query.exec(queryString))
        {
            qCritical()<<"Query error: "<<query.lastError()
                      <<" Query executed: "<<query.lastQuery();
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
            return;
        }
        emit refreshModel();
    }
}

void Materials_Box::deleteRecord()
{
    QSqlQuery q;
    int currentMaterialId = m_model->index(m_table->currentIndex().row(), id).data().toInt();
    QString queryString = QString("SELECT COUNT(*) FROM shoes WHERE material = %1;")
            .arg(currentMaterialId);
    if(q.exec(queryString) && q.next())
    {
        //If there is a relation to shoes table show info that the record can't be deleted
        if(q.value(0).toInt() > 0)
        {
            QMessageBox::critical(this, MSG_ERROR, ERR_EXISTS_RELATION);
            return;
        }

        queryString = QString("DELETE FROM materials WHERE id = %1;").arg(currentMaterialId);
        if(!q.exec(queryString))
        {
            qCritical()<<"Query error: "<<q.lastError()
                      <<" Query executed: "<<q.lastQuery();
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
        }
        emit refreshModel();
    }
}


