#include "CustomTableView.h"

CustomTableView::CustomTableView(QWidget *parent)
    : QTableView(parent)
{
    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);
}
