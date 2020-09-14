#include "MaterialsBox.h"

#include <QLineEdit>
#include <QMessageBox>
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

MaterialsBox::MaterialsBox(QWidget *parent)
    : GeneralDialog(parent)
{
    setWindowTitle(MATERIALS);

    m_model->setHeaderData(id, Qt::Horizontal, ID);
    m_model->setHeaderData(material, Qt::Horizontal, MATERIAL);
    m_model->setQuery(MATERIAL_SEARCH_QUERY.arg(""));

    m_table->hideColumn(id);

    m_leSearch->setPlaceholderText(SEARCH_MATERIALS_PLACEHOLDER);
    m_leNew->setPlaceholderText(INSERT_MATERIAL);
}

void MaterialsBox::newClicked()
{
    if(m_leNew->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, EMPTY_FIELD, MATERIAL_FIELD_EMPTY);
    }
    else
    {
        QString queryString = QString("INSERT INTO materials (material) VALUES('%1')").arg(m_leNew->text());
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

void MaterialsBox::deleteRecord()
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

void MaterialsBox::filterModel()
{
    m_model->setQuery(MATERIAL_SEARCH_QUERY.arg(m_leSearch->text()));
}


