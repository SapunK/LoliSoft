#ifndef POPULATEDATABASE_H
#define POPULATEDATABASE_H

#include <QDebug>

#ifdef QT_DEBUG

class PopulateDatabase
{
public:
    PopulateDatabase();
    static void createDatabase();
    static void fillDatabase();

private:
    static void createColors();
    static void createShoes();
    static void createMaterials();
    static void createModels();
    static void createItems();
    static void createDocuments();
    static void createDocTypes();

    static void fillColors();
    static void fillModels();
    static void fillMaterials();
    static void fillShoes();
};
#endif //QT_DEBUG
#endif // POPULATEDATABASE_H
