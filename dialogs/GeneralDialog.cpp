#include "GeneralDialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

#include "appconsts.h"
#include "custom_widgets/CustomTableView.h"

GeneralDialog::GeneralDialog(QWidget *parent, bool hideEditNew)
    : QDialog(parent),
      m_bHideEditNew(hideEditNew)
{
    setupForm();

    connect(m_pbNew, &QAbstractButton::clicked, this, &GeneralDialog::newClicked);
    connect(m_pbClose, &QAbstractButton::clicked, this, &QDialog::close);
    connect(m_pbDelete, &QPushButton::clicked, this, &GeneralDialog::deleteRecord);
    connect(this, &GeneralDialog::refreshModel, this, [this]{
        m_leNew->clear();
        m_model->setQuery(m_model->query().lastQuery());
        m_table->resizeColumnsToContents();
    });
    connect(m_leSearch, &QLineEdit::returnPressed, this, &GeneralDialog::filterModel);
}

void GeneralDialog::setupForm()
{
    m_model = new QSqlQueryModel(this);

    m_table = new CustomTableView(this);
    m_table->setModel(m_model);

    m_leSearch = new QLineEdit(this);
    setFocusProxy(m_leSearch);
    m_leSearch->setFocus();

    m_leNew = new QLineEdit(this);
    m_leNew->setHidden(m_bHideEditNew);

    m_pbNew = new QPushButton(NEW, this);
    m_pbNew->setFocusPolicy(Qt::NoFocus);

    m_pbClose = new QPushButton(CLOSE, this);
    m_pbClose->setFocusPolicy(Qt::NoFocus);

    m_pbDelete = new QPushButton(DELETE, this);
    m_pbDelete->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *mainLayout = new QHBoxLayout;

    QVBoxLayout *vLeftSide = new QVBoxLayout;
    QVBoxLayout *vRightSide = new QVBoxLayout;

    m_grLayout = new QGridLayout;

    vLeftSide->addWidget(m_leNew);
    vLeftSide->addWidget(m_pbNew);
    vLeftSide->addWidget(m_pbClose);
    vLeftSide->addWidget(m_pbDelete);
    vLeftSide->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    vRightSide->addWidget(m_leSearch);
    vRightSide->addWidget(m_table);

    mainLayout->addLayout(vLeftSide);
    mainLayout->addLayout(m_grLayout);
    mainLayout->addLayout(vRightSide);

    setLayout(mainLayout);
}
