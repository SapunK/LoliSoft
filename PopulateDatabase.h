#ifndef POPULATEDATABASE_H
#define POPULATEDATABASE_H

class PopulateDatabase
{
public:
    PopulateDatabase();
    static void createDatabase();

private:
    static void createColors();
    static void createShoes();
    static void createMaterials();
    static void createModels();
    static void addForeignKeys();
};

#endif // POPULATEDATABASE_H
