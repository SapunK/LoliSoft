#include "Colors_Box.h"

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

namespace Colors_Box_NS{
static const char* SEARCH_COLORS_PLACEHOLDER = "Search by color";
static const char* INSERT_COLOR = "Insert new color";
static const char* COLOR_FIELD_EMPTY = "Color field is empty.\nInsert a new color then press the button.";
static const QString COLOR_SEARCH_QUERY = "SELECT color FROM colors WHERE color ilike '%%1%';";
}

using namespace Colors_Box_NS;

Colors_Box::Colors_Box(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(COLORS);
    setupForm();

    connect(m_pbNew, &QAbstractButton::clicked, this, &Colors_Box::newClicked);
    connect(m_pbClose, &QAbstractButton::clicked, this, &Colors_Box::closeClicked);
    connect(m_leColorSearch, &QLineEdit::returnPressed, this, [this](){
        m_model->setQuery(COLOR_SEARCH_QUERY.arg(m_leColorSearch->text()));
    });
}

void Colors_Box::setupForm()
{
    m_model = new QSqlQueryModel(this);
    m_model->setHeaderData(0, Qt::Horizontal, COLOR);

    m_table = new QTableView(this);
    m_table->resizeColumnsToContents();
    m_table->setSelectionMode(QTableView::SingleSelection);
    m_table->setSelectionBehavior(QTableView::SelectRows);
    m_table->setModel(m_model);

    m_leColorSearch = new QLineEdit(this);
    m_leColorSearch->setPlaceholderText(SEARCH_COLORS_PLACEHOLDER);
    setFocusProxy(m_leColorSearch);
    m_leColorSearch->setFocus();

    m_leNewColor = new QLineEdit(this);
    m_leNewColor->setPlaceholderText(INSERT_COLOR);

    m_pbNew = new QPushButton(NEW, this);
    m_pbNew->setFocusPolicy(Qt::NoFocus);

    m_pbClose = new QPushButton(CLOSE, this);
    m_pbClose->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *vLeftSide = new QVBoxLayout;
    QVBoxLayout *vRightSide = new QVBoxLayout;

    vLeftSide->addWidget(m_leNewColor);
    vLeftSide->addWidget(m_pbNew);
    vLeftSide->addWidget(m_pbClose);
    vLeftSide->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    vRightSide->addWidget(m_leColorSearch);
    vRightSide->addWidget(m_table);

    mainLayout->addLayout(vLeftSide);
    mainLayout->addLayout(vRightSide);

    setLayout(mainLayout);
}

void Colors_Box::newClicked()
{
    if(m_leNewColor->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, EMPTY_FIELD, COLOR_FIELD_EMPTY);
    }
    else
    {
        QString queryString = QString("INSERT INTO colors (color) VALUES('%1')").arg(m_leNewColor->text());
        QSqlQuery query;
        if(!query.exec(queryString))
        {
            qCritical()<<"Query error: "<<query.lastError()
                      <<" Query executed: "<<query.lastQuery();
            QMessageBox::critical(this, MSG_ERROR, ERR_CONTACT_ADMINISTRATOR);
            return;
        }
        m_model->setQuery(m_model->query().lastQuery());
    }
}
void Colors_Box::closeClicked()
{
    reject();
}

