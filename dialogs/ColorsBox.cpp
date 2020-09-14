#include "ColorsBox.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QTableView>
#include <QDebug>

#include "appconsts.h"

namespace Colors_Box_NS{
static const char* SEARCH_COLORS_PLACEHOLDER = "Search by color";
static const char* INSERT_COLOR = "Insert new color";
static const char* COLOR_FIELD_EMPTY = "Color field is empty.\nInsert a new color then press the button.";
static const QString COLOR_SEARCH_QUERY = "SELECT id, color FROM colors WHERE color ilike '%%1%';";
}

using namespace Colors_Box_NS;

ColorsBox::ColorsBox(QWidget *parent)
    : GeneralDialog(parent)
{
    setWindowTitle(COLORS);

    m_model->setHeaderData(id, Qt::Horizontal, ID);
    m_model->setHeaderData(color, Qt::Horizontal, COLOR);
    m_model->setQuery(COLOR_SEARCH_QUERY.arg(""));

    m_table->resizeColumnsToContents();
    m_table->hideColumn(id);

    m_leSearch->setPlaceholderText(SEARCH_COLORS_PLACEHOLDER);
    m_leNew->setPlaceholderText(INSERT_COLOR);
}

void ColorsBox::newClicked()
{
    if(m_leNew->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, EMPTY_FIELD, COLOR_FIELD_EMPTY);
    }
    else
    {
        QString queryString = QString("INSERT INTO colors (color) VALUES('%1')").arg(m_leNew->text());
        QSqlQuery query;
        if(!query.exec(queryString))
        {
            qCritical()<<"Query error: "<<query.lastError()
                      <<" Query executed: "<<query.lastQuery();
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
            return;
        }
        m_leNew->clear();
        emit refreshModel();
    }
}

void ColorsBox::deleteRecord()
{
    QSqlQuery q;
    int currentColorId = m_model->index(m_table->currentIndex().row(), id).data().toInt();
    QString queryString = QString("SELECT COUNT(*) FROM shoes WHERE color = %1;")
            .arg(currentColorId);
    if(q.exec(queryString) && q.next())
    {
        //If there is a relation to shoes table show info to the user that the record can't be deleted
        if(q.value(0).toInt() > 0)
        {
            QMessageBox::critical(this, MSG_ERROR, ERR_EXISTS_RELATION);
            return;
        }

        queryString = QString("DELETE FROM colors WHERE id = %1;").arg(currentColorId);
        if(!q.exec(queryString))
        {
            qCritical()<<"Query error: "<<q.lastError()
                      <<" Query executed: "<<q.lastQuery();
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
        }
        emit refreshModel();
    }
}

void ColorsBox::filterModel()
{
    m_model->setQuery(COLOR_SEARCH_QUERY.arg(m_leSearch->text()));
}

