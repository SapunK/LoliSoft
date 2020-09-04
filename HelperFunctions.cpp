#include "HelperFunctions.h"

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QLineEdit>

#include <appconsts.h>

int HelperFunctions::m_desktopWidth = 0;
int HelperFunctions::m_desktopHeight = 0;


HelperFunctions::HelperFunctions()
{

}

void HelperFunctions::setWidgetProperties(QWidget &widget)
{
    if(widget.metaObject()->className() == QString("QPushButton")){
        widget.setFixedWidth(m_desktopWidth * 0.15);
        widget.setFixedHeight(m_desktopHeight * 0.05);
        widget.setStyleSheet(FONT_WEIGHT_BOLD);
        widget.setStyleSheet(PB_FONTSIZE);
        return;
    }

    if(widget.metaObject()->className() == QString("QLineEdit"))
    {
        widget.setFixedWidth(m_desktopWidth * 0.5);
        widget.setFixedHeight(m_desktopHeight * 0.05);
        qobject_cast<QLineEdit*>(&widget)->setPlaceholderText(SEARCH_PLACEHOLDER_TEXT);
        widget.setStyleSheet(SEARCHLE_FONTSIZE);
    }
}

int HelperFunctions::desktopWidth()
{
    return m_desktopWidth;
}

int HelperFunctions::desktopHeight()
{
    return m_desktopHeight;
}

void HelperFunctions::setDesktopSize()
{
    //FIXME for two or more screens (probably get the dimensions of the main screen)
    m_desktopWidth = qApp->desktop()->width();
    m_desktopHeight = qApp->desktop()->height();
}
