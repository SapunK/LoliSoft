#include "Models_Box.h"

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

namespace Models_Box_NS{
static const char* SEARCH_MODELS_PLACEHOLDER = "Search by model";
static const char* INSERT_MODEL = "Insert new model";
static const char* MODEL_FIELD_EMPTY = "Model field is empty.\nInsert a new model then press the button.";
static const QString MODEL_SEARCH_QUERY = "SELECT id, model FROM models WHERE model ilike '%%1%';";
}

using namespace Models_Box_NS;

Models_Box::Models_Box(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(MATERIALS);
    setupForm();

    connect(m_pbNew, &QAbstractButton::clicked, this, &Models_Box::newClicked);
    connect(m_pbClose, &QAbstractButton::clicked, this, &QDialog::close);
    connect(m_leModelSearch, &QLineEdit::returnPressed, this, [this](){
        m_model->setQuery(MODEL_SEARCH_QUERY.arg(m_leModelSearch->text()));
    });
    connect(m_pbDelete, &QPushButton::clicked, this, &Models_Box::deleteRecord);
    connect(this, &Models_Box::refreshModel, this, [this]{
       m_model->setQuery(m_model->query().lastQuery());
       m_table->resizeColumnsToContents();
    });
}

void Models_Box::setupForm()
{
    m_model = new QSqlQueryModel(this);
    m_model->setHeaderData(id, Qt::Horizontal, ID);
    m_model->setHeaderData(model, Qt::Horizontal, MODEL);
    m_model->setQuery(MODEL_SEARCH_QUERY.arg(""));

    m_table = new QTableView(this);
    m_table->setSelectionMode(QTableView::SingleSelection);
    m_table->setSelectionBehavior(QTableView::SelectRows);
    m_table->setModel(m_model);
    m_table->resizeColumnsToContents();
    m_table->hideColumn(id);

    m_leModelSearch = new QLineEdit(this);
    m_leModelSearch->setPlaceholderText(SEARCH_MODELS_PLACEHOLDER);
    setFocusProxy(m_leModelSearch);
    m_leModelSearch->setFocus();

    m_leNewModel = new QLineEdit(this);
    m_leNewModel->setPlaceholderText(INSERT_MODEL);

    m_pbNew = new QPushButton(NEW, this);
    m_pbNew->setFocusPolicy(Qt::NoFocus);

    m_pbClose = new QPushButton(CLOSE, this);
    m_pbClose->setFocusPolicy(Qt::NoFocus);

    m_pbDelete = new QPushButton(DELETE, this);
    m_pbDelete->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *vLeftSide = new QVBoxLayout;
    QVBoxLayout *vRightSide = new QVBoxLayout;

    vLeftSide->addWidget(m_leNewModel);
    vLeftSide->addWidget(m_pbNew);
    vLeftSide->addWidget(m_pbClose);
    vLeftSide->addWidget(m_pbDelete);
    vLeftSide->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    vRightSide->addWidget(m_leModelSearch);
    vRightSide->addWidget(m_table);

    mainLayout->addLayout(vLeftSide);
    mainLayout->addLayout(vRightSide);

    setLayout(mainLayout);
}

void Models_Box::newClicked()
{
    if(m_leNewModel->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, EMPTY_FIELD, MODEL_FIELD_EMPTY);
    }
    else
    {
        QString queryString = QString("INSERT INTO models (model) VALUES('%1')").arg(m_leNewModel->text());
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

void Models_Box::deleteRecord()
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


