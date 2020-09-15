#include "ModelsBox.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

#include "appconsts.h"
#include "custom_widgets/CustomTableView.h"

namespace Models_Box_NS{
static const char* SEARCH_MODELS_PLACEHOLDER = "Search by model";
static const char* INSERT_MODEL = "Insert new model";
static const char* MODEL_FIELD_EMPTY = "Model field is empty.\nInsert a new model then press the button.";
static const QString MODEL_SEARCH_QUERY = "SELECT id, model FROM models WHERE model ilike '%%1%';";
}

using namespace Models_Box_NS;

ModelsBox::ModelsBox(QWidget *parent)
    : GeneralDialog(parent)
{
    setWindowTitle(MATERIALS);

    m_model->setHeaderData(id, Qt::Horizontal, ID);
    m_model->setHeaderData(model, Qt::Horizontal, MODEL);
    m_model->setQuery(MODEL_SEARCH_QUERY.arg(""));

    m_table->hideColumn(id);

    m_leSearch->setPlaceholderText(SEARCH_MODELS_PLACEHOLDER);
    m_leNew->setPlaceholderText(INSERT_MODEL);

}

void ModelsBox::newClicked()
{
    if(m_leNew->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, EMPTY_FIELD, MODEL_FIELD_EMPTY);
    }
    else
    {
        QString queryString = QString("INSERT INTO models (model) VALUES('%1')").arg(m_leNew->text());
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

void ModelsBox::deleteRecord()
{
    QSqlQuery q;
    int currentMaterialId = m_model->index(m_table->currentIndex().row(), id).data().toInt();
    QString queryString = QString("SELECT COUNT(*) FROM shoes WHERE model = %1;")
            .arg(currentMaterialId);
    if(q.exec(queryString) && q.next())
    {
        //If there is a relation to shoes table show info that the record can't be deleted
        if(q.value(0).toInt() > 0)
        {
            QMessageBox::critical(this, MSG_ERROR, ERR_EXISTS_RELATION);
            return;
        }

        queryString = QString("DELETE FROM models WHERE id = %1;").arg(currentMaterialId);
        if(!q.exec(queryString))
        {
            qCritical()<<"Query error: "<<q.lastError()
                      <<" Query executed: "<<q.lastQuery();
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
        }
        emit refreshModel();
    }
}

void ModelsBox::filterModel()
{
    m_model->setQuery(MODEL_SEARCH_QUERY.arg(m_leSearch->text()));
}


