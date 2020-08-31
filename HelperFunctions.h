#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

class QWidget;

class HelperFunctions
{
public:
    HelperFunctions();
    static void setWidgetProperties(QWidget &widget);

    static int desktopWidth();
    static int desktopHeight();

    static void setDesktopSize();

private:
    static int m_desktopWidth;
    static int m_desktopHeight;
};

#endif // HELPERFUNCTIONS_H
