#include "ViewDocuments.h"

#include <QComboBox>
#include <QSqlQueryModel>
#include <QVBoxLayout>

#include "HelperFunctions.h"
#include "appconsts.h"
#include "custom_widgets/CustomTableView.h"

namespace ViewDocuments_NS {
static const char* Q_CB_MODEL = "SELECT 0, 'all' "
                                "UNION "
                                "SELECT id, doc_name FROM doc_types "
                                "ORDER BY 1;";
static const QString Q_MODEL = "SELECT dt.id, dt.doc_name, d.doc_number, d.entry_price, d.rebate, d.tax, d.sale_price, d.price_difference, d.datetime "
                             "FROM documents d "
                             "INNER JOIN doc_types dt on d.doc_type_id = dt.id %1";
}

using namespace ViewDocuments_NS;

ViewDocuments::ViewDocuments(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(HelperFunctions::desktopWidth() * 0.5, HelperFunctions::desktopHeight() * 0.4);
    setWindowTitle(VIEW_DOCUMENTS);
    setupForm();
}

void ViewDocuments::setupForm()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *lTable = new QVBoxLayout();

    m_cbDocTypes = new QComboBox(this);
    QSqlQueryModel *cbModel = new QSqlQueryModel(this);
    cbModel->setQuery(Q_CB_MODEL);
    m_cbDocTypes->setModel(cbModel);
    m_cbDocTypes->setModelColumn(1);
    m_cbDocTypes->setCurrentIndex(m_cbDocTypes->findText("all"));
    connect(m_cbDocTypes, SIGNAL(currentIndexChanged(int)), this, SLOT(filterModel()));

    HelperFunctions::setWidgetProperties(*m_cbDocTypes);

    m_model = new QSqlQueryModel(this);
    m_table = new CustomTableView(this);

    //TODO set column names
    m_model->setQuery(Q_MODEL.arg(""));
    m_table->setModel(m_model);
    m_table->hideColumn(doc_type_id);

    lTable->addWidget(m_table);

    hLayout->addWidget(m_cbDocTypes);
    hLayout->addSpacerItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(lTable);
}

void ViewDocuments::filterModel()
{
    if(m_cbDocTypes->currentIndex() == 0){
        m_model->setQuery(Q_MODEL.arg(""));
        return;
    }

    QString whereClause = QString("WHERE dt.id = %1").arg(m_cbDocTypes->model()->index(m_cbDocTypes->currentIndex(), 0).data().toInt());
    m_model->setQuery(Q_MODEL.arg(whereClause));
}
